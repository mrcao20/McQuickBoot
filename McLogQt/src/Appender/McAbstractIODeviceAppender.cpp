#include "McLog/Appender/impl/McAbstractIODeviceAppender.h"

#include <QIODevice>
#include <QTextStream>

MC_INIT(McAbstractIODeviceAppender)
MC_INIT_END

MC_DECL_PRIVATE_DATA(McAbstractIODeviceAppender)
QIODevicePtr device;
QTextStream textStream;
MC_DECL_PRIVATE_DATA_END

McAbstractIODeviceAppender::McAbstractIODeviceAppender()
{
    MC_NEW_PRIVATE_DATA(McAbstractIODeviceAppender);
}

McAbstractIODeviceAppender::~McAbstractIODeviceAppender()
{
    if(!d->device.isNull() && d->device->isOpen())
        d->device->close();
}

QIODevicePtr McAbstractIODeviceAppender::device() const noexcept 
{
    return d->device;
}

void McAbstractIODeviceAppender::setDevice(QIODeviceConstPtrRef device) noexcept 
{
    d->device = device;
}

void McAbstractIODeviceAppender::allFinished() noexcept
{
    McAbstractAppender::finished();

    d->textStream.setDevice(d->device.data());
    d->textStream.setCodec("UTF-8");
}

QTextStream &McAbstractIODeviceAppender::textStream() noexcept
{
    return d->textStream;
}
