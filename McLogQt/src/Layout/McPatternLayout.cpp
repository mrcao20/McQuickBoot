#include "McLog/Layout/impl/McPatternLayout.h"

#include <QCoreApplication>
#include <QElapsedTimer>
#include <QDateTime>
#include <QThread>

namespace McPrivate {

static bool isDefaultCategory(const char *category) 
{
    return !category || strcmp(category, "default") == 0;
}

#ifndef QT_BOOTSTRAPPED
#if defined(Q_OS_LINUX) && (defined(__GLIBC__) || QT_HAS_INCLUDE(<sys/syscall.h>))
#  include <sys/syscall.h>
#if  QT_HAS_INCLUDE(<unistd.h>)
#   include <unistd.h>
#endif

# if defined(Q_OS_ANDROID) && !defined(SYS_gettid)
#  define SYS_gettid __NR_gettid
# endif

static long mc_gettid() 
{
    // no error handling
    // this syscall has existed since Linux 2.4.11 and cannot fail
    return syscall(SYS_gettid);
}
#elif defined(Q_OS_DARWIN)
#  include <pthread.h>
static int mc_gettid() 
{
    // no error handling: this call cannot fail
    __uint64_t tid;
    pthread_threadid_np(NULL, &tid);
    return tid;
}
#elif defined(Q_OS_FREEBSD_KERNEL) && defined(__FreeBSD_version) && __FreeBSD_version >= 900031
#  include <pthread_np.h>
static int mc_gettid() 
{
    return pthread_getthreadid_np();
}
#else
static QT_PREPEND_NAMESPACE(qint64) mc_gettid() 
{
    QT_USE_NAMESPACE
    return qintptr(QThread::currentThreadId());
}
#endif

#ifdef QLOGGING_HAVE_BACKTRACE
#  include <qregularexpression.h>
#  include <cxxabi.h>
#  include <execinfo.h>
#endif
#endif // !QT_BOOTSTRAPPED

/*!
    \internal
*/
QByteArray mcCleanupFuncinfo(QByteArray info) 
{
    // Strip the function info down to the base function name
    // note that this throws away the template definitions,
    // the parameter types (overloads) and any const/volatile qualifiers.

    if (info.isEmpty())
        return info;

    int pos;

    // Skip trailing [with XXX] for templates (gcc), but make
    // sure to not affect Objective-C message names.
    pos = info.size() - 1;
    if (info.endsWith(']') && !(info.startsWith('+') || info.startsWith('-'))) {
        while (--pos) {
            if (info.at(pos) == '[')
                info.truncate(pos);
        }
    }

    // operator names with '(', ')', '<', '>' in it
    static const char operator_call[] = "operator()";
    static const char operator_lessThan[] = "operator<";
    static const char operator_greaterThan[] = "operator>";
    static const char operator_lessThanEqual[] = "operator<=";
    static const char operator_greaterThanEqual[] = "operator>=";

    // canonize operator names
    info.replace("operator ", "operator");

    // remove argument list
    forever {
        int parencount = 0;
        pos = info.lastIndexOf(')');
        if (pos == -1) {
            // Don't know how to parse this function name
            return info;
        }

        // find the beginning of the argument list
        --pos;
        ++parencount;
        while (pos && parencount) {
            if (info.at(pos) == ')')
                ++parencount;
            else if (info.at(pos) == '(')
                --parencount;
            --pos;
        }
        if (parencount != 0)
            return info;

        info.truncate(++pos);

        if (info.at(pos - 1) == ')') {
            if (info.indexOf(operator_call) == pos - static_cast<int>(strlen(operator_call)))
                break;

            // this function returns a pointer to a function
            // and we matched the arguments of the return type's parameter list
            // try again
            info.remove(0, info.indexOf('('));
            info.chop(1);
            continue;
        } else {
            break;
        }
    }

    // find the beginning of the function name
    int parencount = 0;
    int templatecount = 0;
    --pos;

    // make sure special characters in operator names are kept
    if (pos > -1) {
        switch (info.at(pos)) {
        case ')':
            if (info.indexOf(operator_call) == pos - static_cast<int>(strlen(operator_call)) + 1)
                pos -= 2;
            break;
        case '<':
            if (info.indexOf(operator_lessThan) == pos - static_cast<int>(strlen(operator_lessThan)) + 1)
                --pos;
            break;
        case '>':
            if (info.indexOf(operator_greaterThan) == pos - static_cast<int>(strlen(operator_greaterThan)) + 1)
                --pos;
            break;
        case '=': {
            int operatorLength = static_cast<int>(strlen(operator_lessThanEqual));
            if (info.indexOf(operator_lessThanEqual) == pos - operatorLength + 1)
                pos -= 2;
            else if (info.indexOf(operator_greaterThanEqual) == pos - operatorLength + 1)
                pos -= 2;
            break;
        }
        default:
            break;
        }
    }

    while (pos > -1) {
        if (parencount < 0 || templatecount < 0)
            return info;

        char c = info.at(pos);
        if (c == ')')
            ++parencount;
        else if (c == '(')
            --parencount;
        else if (c == '>')
            ++templatecount;
        else if (c == '<')
            --templatecount;
        else if (c == ' ' && templatecount == 0 && parencount == 0)
            break;

        --pos;
    }
    info = info.mid(pos + 1);

    // remove trailing '*', '&' that are part of the return argument
    while ((info.at(0) == '*')
           || (info.at(0) == '&'))
        info = info.mid(1);

    // we have the full function name now.
    // clean up the templates
    while ((pos = info.lastIndexOf('>')) != -1) {
        if (!info.contains('<'))
            break;

        // find the matching close
        int end = pos;
        templatecount = 1;
        --pos;
        while (pos && templatecount) {
            char c = info.at(pos);
            if (c == '>')
                ++templatecount;
            else if (c == '<')
                --templatecount;
            --pos;
        }
        ++pos;
        info.remove(pos, end - pos + 1);
    }

    return info;
}

// tokens as recognized in QT_MESSAGE_PATTERN
static const char categoryTokenC[] = "%{category}";
static const char typeTokenC[] = "%{type}";
static const char messageTokenC[] = "%{message}";
static const char fileTokenC[] = "%{file}";
static const char lineTokenC[] = "%{line}";
static const char functionTokenC[] = "%{function}";
static const char pidTokenC[] = "%{pid}";
static const char appnameTokenC[] = "%{appname}";
static const char threadidTokenC[] = "%{threadid}";
static const char qthreadptrTokenC[] = "%{qthreadptr}";
static const char timeTokenC[] = "%{time"; //not a typo: this command has arguments
static const char backtraceTokenC[] = "%{backtrace"; //ditto
static const char ifCategoryTokenC[] = "%{if-category}";
static const char ifDebugTokenC[] = "%{if-debug}";
static const char ifInfoTokenC[] = "%{if-info}";
static const char ifWarningTokenC[] = "%{if-warning}";
static const char ifCriticalTokenC[] = "%{if-critical}";
static const char ifFatalTokenC[] = "%{if-fatal}";
static const char endifTokenC[] = "%{endif}";
static const char emptyTokenC[] = "";

static const char defaultPattern[] = "%{if-category}%{category}: %{endif}%{message}";


struct McMessagePattern 
{
    McMessagePattern();
    ~McMessagePattern();

    void setPattern(const QString &pattern);

    // 0 terminated arrays of literal tokens / literal or placeholder tokens
    const char **literals;
    const char **tokens;
    QList<QString> timeArgs;   // timeFormats in sequence of %{time
#ifndef QT_BOOTSTRAPPED
    QElapsedTimer timer;
#endif
#ifdef QLOGGING_HAVE_BACKTRACE
    struct BacktraceParams 
    {
        QString backtraceSeparator;
        int backtraceDepth;
    };
    QVector<BacktraceParams> backtraceArgs; // backtrace argumens in sequence of %{backtrace
#endif
};
MC_DECL_POINTER(McMessagePattern)
#ifdef QLOGGING_HAVE_BACKTRACE
Q_DECLARE_TYPEINFO(McMessagePattern::BacktraceParams, Q_MOVABLE_TYPE);
#endif

McMessagePattern::McMessagePattern()
    : literals(nullptr)
    , tokens(nullptr)
{
#ifndef QT_BOOTSTRAPPED
    timer.start();
#endif
    setPattern(QLatin1String(defaultPattern));
}

McMessagePattern::~McMessagePattern() 
{
    for (int i = 0; literals[i]; ++i)
        delete [] literals[i];
    delete [] literals;
    literals = nullptr;
    delete [] tokens;
    tokens = nullptr;
}

void McMessagePattern::setPattern(const QString &pattern) 
{
    if (literals) {
        for (int i = 0; literals[i]; ++i)
            delete [] literals[i];
        delete [] literals;
    }
    delete [] tokens;
    timeArgs.clear();
#ifdef QLOGGING_HAVE_BACKTRACE
    backtraceArgs.clear();
#endif

    // scanner
    QList<QString> lexemes;
    QString lexeme;
    bool inPlaceholder = false;
    for (int i = 0; i < pattern.size(); ++i) {
        const QChar c = pattern.at(i);
        if ((c == QLatin1Char('%'))
                && !inPlaceholder) {
            if ((i + 1 < pattern.size())
                    && pattern.at(i + 1) == QLatin1Char('{')) {
                // beginning of placeholder
                if (!lexeme.isEmpty()) {
                    lexemes.append(lexeme);
                    lexeme.clear();
                }
                inPlaceholder = true;
            }
        }

        lexeme.append(c);

        if ((c == QLatin1Char('}') && inPlaceholder)) {
            // end of placeholder
            lexemes.append(lexeme);
            lexeme.clear();
            inPlaceholder = false;
        }
    }
    if (!lexeme.isEmpty())
        lexemes.append(lexeme);

    // tokenizer
    QVarLengthArray<const char*> literalsVar;
    tokens = new const char*[static_cast<qulonglong>(lexemes.size() + 1)];
    tokens[lexemes.size()] = nullptr;

    bool nestedIfError = false;
    bool inIf = false;
    QString error;

    for (int i = 0; i < lexemes.size(); ++i) {
        const QString lexeme = lexemes.at(i);
        if (lexeme.startsWith(QLatin1String("%{"))
                && lexeme.endsWith(QLatin1Char('}'))) {
            // placeholder
            if (lexeme == QLatin1String(typeTokenC)) {
                tokens[i] = typeTokenC;
            } else if (lexeme == QLatin1String(categoryTokenC))
                tokens[i] = categoryTokenC;
            else if (lexeme == QLatin1String(messageTokenC))
                tokens[i] = messageTokenC;
            else if (lexeme == QLatin1String(fileTokenC))
                tokens[i] = fileTokenC;
            else if (lexeme == QLatin1String(lineTokenC))
                tokens[i] = lineTokenC;
            else if (lexeme == QLatin1String(functionTokenC))
                tokens[i] = functionTokenC;
            else if (lexeme == QLatin1String(pidTokenC))
                tokens[i] = pidTokenC;
            else if (lexeme == QLatin1String(appnameTokenC))
                tokens[i] = appnameTokenC;
            else if (lexeme == QLatin1String(threadidTokenC))
                tokens[i] = threadidTokenC;
            else if (lexeme == QLatin1String(qthreadptrTokenC))
                tokens[i] = qthreadptrTokenC;
            else if (lexeme.startsWith(QLatin1String(timeTokenC))) {
                tokens[i] = timeTokenC;
                int spaceIdx = lexeme.indexOf(QChar::fromLatin1(' '));
                if (spaceIdx > 0)
                    timeArgs.append(lexeme.mid(spaceIdx + 1, lexeme.length() - spaceIdx - 2));
                else
                    timeArgs.append(QString());
            } else if (lexeme.startsWith(QLatin1String(backtraceTokenC))) {
#ifdef QLOGGING_HAVE_BACKTRACE
                tokens[i] = backtraceTokenC;
                QString backtraceSeparator = QStringLiteral("|");
                int backtraceDepth = 5;
                static const QRegularExpression depthRx(QStringLiteral(" depth=(?|\"([^\"]*)\"|([^ }]*))"));
                static const QRegularExpression separatorRx(QStringLiteral(" separator=(?|\"([^\"]*)\"|([^ }]*))"));
                QRegularExpressionMatch m = depthRx.match(lexeme);
                if (m.hasMatch()) {
                    int depth = m.capturedRef(1).toInt();
                    if (depth <= 0)
                        error += QLatin1String("QT_MESSAGE_PATTERN: %{backtrace} depth must be a number greater than 0\n");
                    else
                        backtraceDepth = depth;
                }
                m = separatorRx.match(lexeme);
                if (m.hasMatch())
                    backtraceSeparator = m.captured(1);
                BacktraceParams backtraceParams;
                backtraceParams.backtraceDepth = backtraceDepth;
                backtraceParams.backtraceSeparator = backtraceSeparator;
                backtraceArgs.append(backtraceParams);
#else
                error += QLatin1String("QT_MESSAGE_PATTERN: %{backtrace} is not supported by this Qt build\n");
                tokens[i] = "";
#endif
            }

#define IF_TOKEN(LEVEL) \
            else if (lexeme == QLatin1String(LEVEL)) { \
                if (inIf) \
                    nestedIfError = true; \
                tokens[i] = LEVEL; \
                inIf = true; \
            }
            IF_TOKEN(ifCategoryTokenC)
            IF_TOKEN(ifDebugTokenC)
            IF_TOKEN(ifInfoTokenC)
            IF_TOKEN(ifWarningTokenC)
            IF_TOKEN(ifCriticalTokenC)
            IF_TOKEN(ifFatalTokenC)
#undef IF_TOKEN
            else if (lexeme == QLatin1String(endifTokenC)) {
                tokens[i] = endifTokenC;
                if (!inIf && !nestedIfError)
                    error += QLatin1String("QT_MESSAGE_PATTERN: %{endif} without an %{if-*}\n");
                inIf = false;
            } else {
                tokens[i] = emptyTokenC;
                error += QStringLiteral("QT_MESSAGE_PATTERN: Unknown placeholder %1\n")
                        .arg(lexeme);
            }
        } else {
            char *literal = new char[static_cast<qulonglong>(lexeme.size() + 1)];
            strncpy(literal, lexeme.toLatin1().constData(), static_cast<size_t>(lexeme.size()));
            literal[lexeme.size()] = '\0';
            literalsVar.append(literal);
            tokens[i] = literal;
        }
    }
    if (nestedIfError)
        error += QLatin1String("QT_MESSAGE_PATTERN: %{if-*} cannot be nested\n");
    else if (inIf)
        error += QLatin1String("QT_MESSAGE_PATTERN: missing %{endif}\n");

    if (!error.isEmpty())
        MC_PRINT_ERR("%s", qPrintable(error));

    literals = new const char*[static_cast<qulonglong>(literalsVar.size() + 1)];
    literals[literalsVar.size()] = nullptr;
    memcpy(literals, literalsVar.constData(), static_cast<qulonglong>(literalsVar.size()) * sizeof(const char*));
}

QString format(McMessagePatternPtr pattern, QtMsgType type, const QMessageLogContext &context, const QString &str) noexcept 
{
    QString message;

    if (!pattern) {
        // after destruction of static QMessagePattern instance
        message.append(str);
        return message;
    }

    bool skip = false;

#ifndef QT_BOOTSTRAPPED
    int timeArgsIdx = 0;
#ifdef QLOGGING_HAVE_BACKTRACE
    int backtraceArgsIdx = 0;
#endif
#endif

    // we do not convert file, function, line literals to local encoding due to overhead
    for (int i = 0; pattern->tokens[i] != nullptr; ++i) {
        const char *token = pattern->tokens[i];
        if (token == endifTokenC) {
            skip = false;
        } else if (skip) {
            // we skip adding messages, but we have to iterate over
            // timeArgsIdx and backtraceArgsIdx anyway
#ifndef QT_BOOTSTRAPPED
            if (token == timeTokenC)
                timeArgsIdx++;
#ifdef QLOGGING_HAVE_BACKTRACE
            else if (token == backtraceTokenC)
                backtraceArgsIdx++;
#endif
#endif
        } else if (token == messageTokenC) {
            message.append(str);
        } else if (token == categoryTokenC) {
            message.append(QLatin1String(context.category));
        } else if (token == typeTokenC) {
            switch (type) {
            case QtDebugMsg:   message.append(QLatin1String("debug")); break;
            case QtInfoMsg:    message.append(QLatin1String("info")); break;
            case QtWarningMsg: message.append(QLatin1String("warning")); break;
            case QtCriticalMsg:message.append(QLatin1String("critical")); break;
            case QtFatalMsg:   message.append(QLatin1String("fatal")); break;
            }
        } else if (token == fileTokenC) {
            if (context.file)
                message.append(QLatin1String(context.file));
            else
                message.append(QLatin1String("unknown"));
        } else if (token == lineTokenC) {
            message.append(QString::number(context.line));
        } else if (token == functionTokenC) {
            if (context.function)
                message.append(QString::fromLatin1(mcCleanupFuncinfo(context.function)));
            else
                message.append(QLatin1String("unknown"));
#ifndef QT_BOOTSTRAPPED
        } else if (token == pidTokenC) {
            message.append(QString::number(QCoreApplication::applicationPid()));
        } else if (token == appnameTokenC) {
            message.append(QCoreApplication::applicationName());
        } else if (token == threadidTokenC) {
            // print the TID as decimal
            message.append(QString::number(mc_gettid()));
        } else if (token == qthreadptrTokenC) {
            message.append(QLatin1String("0x"));
            message.append(QString::number(qlonglong(QThread::currentThread()->currentThread()), 16));
#ifdef QLOGGING_HAVE_BACKTRACE
        } else if (token == backtraceTokenC) {
            QMessagePattern::BacktraceParams backtraceParams = pattern->backtraceArgs.at(backtraceArgsIdx);
            backtraceArgsIdx++;
            message.append(formatBacktraceForLogMessage(backtraceParams, context.function));
#endif
        } else if (token == timeTokenC) {
            QString timeFormat = pattern->timeArgs.at(timeArgsIdx);
            timeArgsIdx++;
            if (timeFormat == QLatin1String("process")) {
                    quint64 ms = static_cast<quint64>(pattern->timer.elapsed());
                    message.append(QString::asprintf("%6d.%03d", uint(ms / 1000), uint(ms % 1000)));
            } else if (timeFormat ==  QLatin1String("boot")) {
                // just print the milliseconds since the elapsed timer reference
                // like the Linux kernel does
                QElapsedTimer now;
                now.start();
                uint ms = static_cast<uint>(now.msecsSinceReference());
                message.append(QString::asprintf("%6d.%03d", uint(ms / 1000), uint(ms % 1000)));
#if QT_CONFIG(datestring)
            } else if (timeFormat.isEmpty()) {
                    message.append(QDateTime::currentDateTime().toString(Qt::ISODate));
            } else {
                message.append(QDateTime::currentDateTime().toString(timeFormat));
#endif // QT_CONFIG(datestring)
            }
#endif // !QT_BOOTSTRAPPED
        } else if (token == ifCategoryTokenC) {
            if (isDefaultCategory(context.category))
                skip = true;
#define HANDLE_IF_TOKEN(LEVEL)  \
        } else if (token == if##LEVEL##TokenC) { \
            skip = type != Qt##LEVEL##Msg;
        HANDLE_IF_TOKEN(Debug)
        HANDLE_IF_TOKEN(Info)
        HANDLE_IF_TOKEN(Warning)
        HANDLE_IF_TOKEN(Critical)
        HANDLE_IF_TOKEN(Fatal)
#undef HANDLE_IF_TOKEN
        } else {
            message.append(QLatin1String(token));
        }
    }
    return message;
}

}

MC_DECL_PRIVATE_DATA(McPatternLayout)
QString pattern;
McPrivate::McMessagePatternPtr messagePattern;
MC_DECL_PRIVATE_DATA_END

MC_INIT(McPatternLayout)
MC_REGISTER_BEAN_FACTORY(MC_TYPELIST(McPatternLayout))
MC_INIT_END

McPatternLayout::McPatternLayout() 
{
    MC_NEW_PRIVATE_DATA(McPatternLayout);
    
    d->messagePattern = McPrivate::McMessagePatternPtr::create();
}

McPatternLayout::~McPatternLayout() 
{
}

QString McPatternLayout::getPattern() const noexcept 
{
    return d->pattern;
}

void McPatternLayout::setPattern(const QString &val) noexcept 
{
    d->pattern = val;
    
    d->messagePattern->setPattern(val);
}

QString McPatternLayout::format(QtMsgType type, const QMessageLogContext &context, const QString &str) noexcept 
{
    return McPrivate::format(d->messagePattern, type, context, str);
}

void McPatternLayout::finished() noexcept 
{
}
