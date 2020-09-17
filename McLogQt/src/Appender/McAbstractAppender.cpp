#include "McLog/Appender/impl/McAbstractAppender.h"

#include <QCoreApplication>
#include <QIODevice>
#include <QThread>

#include "McLog/Layout/impl/McNormalLayout.h"

#define LEVEL_DEBUG "debug"
#define LEVEL_WARN "warn"
#define LEVEL_CRITICAL "critical"
#define LEVEL_FATAL "fatal"
#define LEVEL_INFO "info"

const QEvent::Type McAppenderEvent::eventType = 
        static_cast<QEvent::Type>(QEvent::registerEventType(QEvent::User + 1));

McAppenderEvent::~McAppenderEvent() 
{
}

MC_DECL_PRIVATE_DATA(McAbstractAppender)
IMcLayoutPtr layout;
QString threshold;                  //!< 全小写
QList<QtMsgType> types;
QIODevicePtr device;
bool immediateFlush{false};         //!< 是否立即刷新输出，默认为false
MC_PADDING_CLANG(7)
MC_DECL_PRIVATE_DATA_END

MC_INIT(McAbstractAppender)
MC_INIT_END

McAbstractAppender::McAbstractAppender() 
{
    MC_NEW_PRIVATE_DATA(McAbstractAppender);
}

McAbstractAppender::~McAbstractAppender() 
{
    if(!d->device.isNull() && d->device->isOpen())
        d->device->close();
}

IMcLayoutPtr McAbstractAppender::layout() const noexcept 
{
    return d->layout;
}

void McAbstractAppender::setLayout(IMcLayoutConstPtrRef val) noexcept 
{
    d->layout = val;
}

QString McAbstractAppender::threshold() const noexcept 
{
    return d->threshold;
}

void McAbstractAppender::setThreshold(const QString &val) noexcept 
{
    d->threshold = val.toLower();
    
    d->types = initThreshold(d->threshold);
}

bool McAbstractAppender::immediateFlush() const noexcept 
{
    return d->immediateFlush;
}

void McAbstractAppender::setImmediateFlush(bool val) noexcept 
{
    d->immediateFlush = val;
}

void McAbstractAppender::append(QtMsgType type, const QMessageLogContext &context, const QString &str) noexcept 
{
    if(!d->types.contains(type)) {
        return;
    }
    auto l = layout();
    if(l.isNull()) {
        MC_PRINT_ERR("the appender not set layout. category: %s\n", context.category);
        return;
    }
    //! 如果没有定义QT_MESSAGELOGCONTEXT
    if (context.file == nullptr
        && context.line == 0
        && context.function == nullptr) {
        MC_PRINT_ERR("in release, need to manual define QT_MESSAGELOGCONTEXT\n");
    }
    auto message = l->format(type, context, str);
    
    if(d->immediateFlush) {
        if(thread() == QThread::currentThread()) {
            append_helper(message.toLocal8Bit());
        }else{
            QMetaObject::invokeMethod(this
                                      , "append_helper"
                                      , Qt::BlockingQueuedConnection
                                      , Q_ARG(QByteArray, message.toLocal8Bit()));
        }
    }else{
        qApp->postEvent(this, new McAppenderEvent(message));
    }
}

QIODevicePtr McAbstractAppender::device() const noexcept 
{
    return d->device;
}

void McAbstractAppender::setDevice(QIODeviceConstPtrRef device) noexcept 
{
    d->device = device;
}

void McAbstractAppender::finished() noexcept 
{
    if(layout().isNull()) {
        auto l = McNormalLayoutPtr::create();
        l->finished();
        setLayout(l);
    }
}

void McAbstractAppender::threadFinished() noexcept 
{
    auto l = layout();  //!< 一定存在
    auto pl = l.staticCast<McPatternLayout>();  //!< 一定成功
    if(pl->thread() != thread()) {
        pl->moveToThread(thread());
    }
}

void McAbstractAppender::customEvent(QEvent *event) 
{
    if(event->type() == McAppenderEvent::eventType) {
        auto e = static_cast<McAppenderEvent *>(event);
        append_helper(e->msg().toLocal8Bit());
    }
}

void McAbstractAppender::append_helper(const QByteArray &msg) noexcept 
{
    auto out = device();
    if(out.isNull() || !out->isOpen()) {
        return;
    }
    out->write(msg);
    out->write("\n", 1);
    flush();
}

QList<QtMsgType> McAbstractAppender::initThreshold(const QString &val) const noexcept 
{
    auto typeStr = val.simplified();
    QList<QtMsgType> types;
    
    if(!typeStr.endsWith('-') && !typeStr.startsWith('-')) {
        auto list = typeStr.split(',');
        for(auto str : list) {
            auto type = strToEnum(str.simplified());
            if(type != -1) {
                types.append(static_cast<QtMsgType>(type));
            }
        }
        return types;
    }else if(typeStr.startsWith('-')) {
        auto level = typeStr.simplified();
        level.remove('-');
        auto type = strToEnum(level);
        
        types.append(static_cast<QtMsgType>(type));
        if (level == LEVEL_DEBUG)
            return types;
        types.append(QtMsgType::QtDebugMsg);
        if (level == LEVEL_WARN)
            return types;
        types.append(QtMsgType::QtWarningMsg);
        if (level == LEVEL_CRITICAL)
            return types;
        types.append(QtMsgType::QtCriticalMsg);
        if (level == LEVEL_FATAL)
            return types;
        types.append(QtMsgType::QtFatalMsg);
        if (level != LEVEL_INFO) {
            types.clear();
            return types;
        }
        
        return types;
    }else{
        auto level = typeStr.simplified();
        level.remove('-');
        auto type = strToEnum(level);
        
        types.append(static_cast<QtMsgType>(type));
#if QT_VERSION >= QT_VERSION_CHECK(5, 5, 0)
        if (level == LEVEL_INFO)
            return types;
        types.append(QtMsgType::QtInfoMsg);
#endif
        if (level == LEVEL_FATAL)
            return types;
        types.append(QtMsgType::QtFatalMsg);
        if (level == LEVEL_CRITICAL)
            return types;
        types.append(QtMsgType::QtCriticalMsg);
        if (level == LEVEL_WARN)
            return types;
        types.append(QtMsgType::QtWarningMsg);
        if (level != LEVEL_DEBUG) {
            types.clear();
            return types;
        }
        
        return types;
    }
}

int McAbstractAppender::strToEnum(const QString &val) const noexcept 
{
    auto type = val.simplified();
    if (type == LEVEL_DEBUG)
        return QtMsgType::QtDebugMsg;
    else if (type == LEVEL_WARN)
        return QtMsgType::QtWarningMsg;
    else if (type == LEVEL_CRITICAL)
        return QtMsgType::QtCriticalMsg;
    else if (type == LEVEL_FATAL)
        return QtMsgType::QtFatalMsg;
#if QT_VERSION >= QT_VERSION_CHECK(5, 5, 0)
    else if(type == LEVEL_INFO)
        return QtMsgType::QtInfoMsg;
#endif
    return -1;
}
