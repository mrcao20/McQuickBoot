/*
 * MIT License
 *
 * Copyright (c) 2021 mrcao20
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "McMessagePattern.h"

#include <QCoreApplication>
#include <QDateTime>
#include <QThread>

namespace McPrivate {
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
static const char timeTokenC[] = "%{time"; // not a typo: this command has arguments
static const char ifCategoryTokenC[] = "%{if-category}";
static const char ifDebugTokenC[] = "%{if-debug}";
static const char ifInfoTokenC[] = "%{if-info}";
static const char ifWarningTokenC[] = "%{if-warning}";
static const char ifCriticalTokenC[] = "%{if-critical}";
static const char ifFatalTokenC[] = "%{if-fatal}";
static const char endifTokenC[] = "%{endif}";
static const char emptyTokenC[] = "";

static const char defaultPattern[] = "%{if-category}%{category}: %{endif}%{message}";

MessagePattern::MessagePattern() noexcept
{
    timer.start();
    setPattern(QLatin1String(defaultPattern));
}

MessagePattern::~MessagePattern() = default;

void MessagePattern::setPattern(const QString &pattern) noexcept
{
    timeArgs.clear();

    QList<QString> lexemes;
    QString lexeme;
    bool inPlaceholder = false;
    for (int i = 0; i < pattern.size(); ++i) {
        const QChar c = pattern.at(i);
        if ((c == QLatin1Char('%')) && !inPlaceholder) {
            if ((i + 1 < pattern.size()) && pattern.at(i + 1) == QLatin1Char('{')) {
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
    std::vector<std::unique_ptr<const char[]>> literalsVar;
    tokens.reset(new const char *[lexemes.size() + 1]);
    tokens[lexemes.size()] = nullptr;

    bool nestedIfError = false;
    bool inIf = false;
    QString error;

    for (int i = 0; i < lexemes.size(); ++i) {
        const QString lexeme = lexemes.at(i);
        if (lexeme.startsWith(QLatin1String("%{")) && lexeme.endsWith(QLatin1Char('}'))) {
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
            }

#define IF_TOKEN(LEVEL) \
 else if (lexeme == QLatin1String(LEVEL)) \
 { \
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
            else if (lexeme == QLatin1String(endifTokenC))
            {
                tokens[i] = endifTokenC;
                if (!inIf && !nestedIfError)
                    error += QLatin1String("QT_MESSAGE_PATTERN: %{endif} without an %{if-*}\n");
                inIf = false;
            }
            else
            {
                tokens[i] = emptyTokenC;
                error += QStringLiteral("QT_MESSAGE_PATTERN: Unknown placeholder %1\n").arg(lexeme);
            }
        } else {
            auto size = lexeme.size() + 1;
            char *literal = new char[size];
            strncpy_s(literal, size, lexeme.toLatin1().constData(), lexeme.size());
            literal[lexeme.size()] = '\0';
            literalsVar.emplace_back(literal);
            tokens[i] = literal;
        }
    }
    if (nestedIfError)
        error += QLatin1String("QT_MESSAGE_PATTERN: %{if-*} cannot be nested\n");
    else if (inIf)
        error += QLatin1String("QT_MESSAGE_PATTERN: missing %{endif}\n");

    if (!error.isEmpty())
        MC_PRINT_ERR("%s", error.toLocal8Bit().constData());

    literals.reset(new std::unique_ptr<const char[]>[literalsVar.size() + 1]);
    std::move(literalsVar.begin(), literalsVar.end(), &literals[0]);
}

QByteArray cleanupFuncinfo(QByteArray info)
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
    Q_FOREVER
    {
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
            if (info.indexOf(operator_call) == pos - (int)strlen(operator_call))
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
            if (info.indexOf(operator_call) == pos - (int)strlen(operator_call) + 1)
                pos -= 2;
            break;
        case '<':
            if (info.indexOf(operator_lessThan) == pos - (int)strlen(operator_lessThan) + 1)
                --pos;
            break;
        case '>':
            if (info.indexOf(operator_greaterThan) == pos - (int)strlen(operator_greaterThan) + 1)
                --pos;
            break;
        case '=': {
            int operatorLength = (int)strlen(operator_lessThanEqual);
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
    while ((info.at(0) == '*') || (info.at(0) == '&'))
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

#if defined(Q_OS_LINUX) && (defined(__GLIBC__) || __has_include(<sys/syscall.h>))
# include <sys/syscall.h>

# if defined(Q_OS_ANDROID) && !defined(SYS_gettid)
#  define SYS_gettid __NR_gettid
# endif

static long gettid()
{
    // no error handling
    // this syscall has existed since Linux 2.4.11 and cannot fail
    return syscall(SYS_gettid);
}
#elif defined(Q_OS_DARWIN)
# include <pthread.h>
static int gettid()
{
    // no error handling: this call cannot fail
    __uint64_t tid;
    pthread_threadid_np(NULL, &tid);
    return tid;
}
#elif defined(Q_OS_FREEBSD_KERNEL) && defined(__FreeBSD_version) && __FreeBSD_version >= 900031
# include <pthread_np.h>
static int gettid()
{
    return pthread_getthreadid_np();
}
#else
static QT_PREPEND_NAMESPACE(qint64) gettid()
{
    QT_USE_NAMESPACE
    return qintptr(QThread::currentThreadId());
}
#endif

static bool isDefaultCategory(const char *category)
{
    return !category || strcmp(category, "default") == 0;
}

QString format(
    const MessagePatternPtr &pattern, QtMsgType type, const QMessageLogContext &context, const QString &str) noexcept
{
    QString message;

    if (pattern.isNull()) {
        message.append(str);
        return message;
    }

    bool skip = false;

    int timeArgsIdx = 0;

    // we do not convert file, function, line literals to local encoding due to overhead
    for (int i = 0; pattern->tokens[i]; ++i) {
        const char *token = pattern->tokens[i];
        if (token == endifTokenC) {
            skip = false;
        } else if (skip) {
            if (token == timeTokenC)
                timeArgsIdx++;
        } else if (token == messageTokenC) {
            message.append(str);
        } else if (token == categoryTokenC) {
            message.append(QLatin1String(context.category));
        } else if (token == typeTokenC) {
            switch (type) {
            case QtDebugMsg:
                message.append(QLatin1String("DEBUG"));
                break;
            case QtInfoMsg:
                message.append(QLatin1String("INFO"));
                break;
            case QtWarningMsg:
                message.append(QLatin1String("WARNING"));
                break;
            case QtCriticalMsg:
                message.append(QLatin1String("CRITICAL"));
                break;
            case QtFatalMsg:
                message.append(QLatin1String("FATAL"));
                break;
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
                message.append(QString::fromLatin1(cleanupFuncinfo(context.function)));
            else
                message.append(QLatin1String("unknown"));
        } else if (token == pidTokenC) {
            message.append(QString::number(QCoreApplication::applicationPid()));
        } else if (token == appnameTokenC) {
            message.append(QCoreApplication::applicationName());
        } else if (token == threadidTokenC) {
            // print the TID as decimal
            message.append(QString::number(gettid()));
        } else if (token == qthreadptrTokenC) {
            message.append(QLatin1String("0x"));
            message.append(QString::number(qlonglong(QThread::currentThread()->currentThread()), 16));
        } else if (token == timeTokenC) {
            QString timeFormat = pattern->timeArgs.at(timeArgsIdx);
            timeArgsIdx++;
            if (timeFormat == QLatin1String("process")) {
                quint64 ms = pattern->timer.elapsed();
                message.append(QString::asprintf("%6d.%03d", uint(ms / 1000), uint(ms % 1000)));
            } else if (timeFormat == QLatin1String("boot")) {
                // just print the milliseconds since the elapsed timer reference
                // like the Linux kernel does
                uint ms = QDeadlineTimer::current().deadline();
                message.append(QString::asprintf("%6d.%03d", uint(ms / 1000), uint(ms % 1000)));
            } else if (timeFormat.isEmpty()) {
                message.append(QDateTime::currentDateTime().toString(Qt::ISODate));
            } else {
                message.append(QDateTime::currentDateTime().toString(timeFormat));
            }
        } else if (token == ifCategoryTokenC) {
            if (isDefaultCategory(context.category))
                skip = true;
#define HANDLE_IF_TOKEN(LEVEL) \
 } \
 else if (token == if##LEVEL##TokenC) \
 { \
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
} // namespace McPrivate
