#include "McLog/Appender/impl/McAbstractFormatAppender.h"

#include "McLog/Layout/impl/McNormalLayout.h"

MC_DECL_PRIVATE_DATA(McAbstractFormatAppender)
IMcLayoutPtr layout;
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

void McAbstractFormatAppender::finished() noexcept
{
    McAbstractAppender::finished();
    
    if(layout().isNull()) {
        auto l = McNormalLayoutPtr::create();
        l->finished();
        setLayout(l);
    }
}

void McAbstractFormatAppender::threadFinished() noexcept
{
    McAbstractAppender::threadFinished();
    
    auto l = layout();  //!< 一定存在
    auto pl = l.staticCast<McPatternLayout>();  //!< 一定成功
    if(pl->thread() != thread()) {
        pl->moveToThread(thread());
    }
}

void McAbstractFormatAppender::doAppend(QtMsgType type, const QMessageLogContext &context, const QString &str) noexcept
{
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
    
    writeBefore();
    auto out = device();
    if(out.isNull() || !out->isOpen()) {
        return;
    }
    out->write(message.toLocal8Bit());
    out->write("\n", 1);
    writeAfter();
}
