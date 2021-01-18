#include "McLog/Appender/impl/McAbstractFormatAppender.h"

#include <QCoreApplication>
#include <QThread>
#include <qlogging.h>

#include "McLog/Layout/impl/McNormalLayout.h"

MC_DECL_PRIVATE_DATA(McAbstractFormatAppender)
IMcLayoutPtr layout;
bool immediateFlush{false};         //!< 是否立即刷新输出，默认为false
MC_DECL_PRIVATE_DATA_END

MC_INIT(McAbstractFormatAppender)
MC_INIT_END

McAbstractFormatAppender::McAbstractFormatAppender() 
{
    MC_NEW_PRIVATE_DATA(McAbstractFormatAppender);
}

McAbstractFormatAppender::~McAbstractFormatAppender() 
{
}

IMcLayoutPtr McAbstractFormatAppender::layout() const noexcept 
{
    return d->layout;
}

void McAbstractFormatAppender::setLayout(IMcLayoutConstPtrRef val) noexcept 
{
    d->layout = val;
}

bool McAbstractFormatAppender::immediateFlush() const noexcept 
{
    return d->immediateFlush;
}

void McAbstractFormatAppender::setImmediateFlush(bool val) noexcept 
{
    d->immediateFlush = val;
}

void McAbstractFormatAppender::append(QtMsgType type, const QMessageLogContext &context, const QString &str) noexcept 
{
    if(!types().contains(type)) {
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
            append_helper(message);
        }else{
            QMetaObject::invokeMethod(this
                                      , MC_STRINGIFY(append_helper)
                                      , Qt::BlockingQueuedConnection
                                      , Q_ARG(QString, message));
        }
    }else{
        auto e = new McCustomEvent(QEvent::User, message);
        qApp->postEvent(this, e);
    }
}

void McAbstractFormatAppender::finished() noexcept
{
    McAbstractIODeviceAppender::finished();
    
    if(layout().isNull()) {
        auto l = McNormalLayoutPtr::create();
        l->finished();
        setLayout(l);
    }
}

void McAbstractFormatAppender::threadFinished() noexcept
{
    McAbstractIODeviceAppender::threadFinished();
    
    auto l = layout();  //!< 一定存在
    auto pl = l.staticCast<McPatternLayout>();  //!< 一定成功
    if(pl->thread() != thread()) {
        pl->moveToThread(thread());
    }
}

void McAbstractFormatAppender::customEvent(QEvent *event) 
{
    if(event->type() == QEvent::User) {
        auto e = static_cast<McCustomEvent *>(event);
        append_helper(e->data().toString());
    }
}

void McAbstractFormatAppender::append_helper(const QString &msg) noexcept
{
    writeBefore();
    auto out = device();
    if(out.isNull() || !out->isOpen()) {
        return;
    }
    out->write(msg.toLocal8Bit());
#ifdef Q_OS_WIN
    QByteArray newLine = "\r\n";
#else
    QByteArray newLine = "\n";
#endif
    out->write(newLine);
    writeAfter();
}
