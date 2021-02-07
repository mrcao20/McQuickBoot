#include "McLog/Appender/impl/McAbstractIODeviceAppender.h"

#include <QIODevice>
#include <QTextCodec>
#include <QTextStream>

MC_INIT(McAbstractIODeviceAppender)
MC_INIT_END

MC_DECL_PRIVATE_DATA(McAbstractIODeviceAppender)
QIODevicePtr device;
QTextStream textStream;
QByteArray codecName{"UTF-8"};
QTextCodec *codec{nullptr};
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

QByteArray McAbstractIODeviceAppender::codecName() const noexcept
{
    return d->codecName;
}

void McAbstractIODeviceAppender::setCodecName(const QByteArray &val) noexcept
{
    d->codecName = val;
}

QTextCodec *McAbstractIODeviceAppender::codec() const noexcept
{
    return d->codec;
}

void McAbstractIODeviceAppender::setCodec(QTextCodec *val) noexcept
{
    d->codec = val;
}

void McAbstractIODeviceAppender::allFinished() noexcept
{
    McAbstractAppender::allFinished();

    d->textStream.setDevice(d->device.data());
    if (d->codec == nullptr) {
        d->codec = QTextCodec::codecForName(d->codecName);
    }
    d->textStream.setCodec(d->codec);
}

QTextStream &McAbstractIODeviceAppender::textStream() noexcept
{
    return d->textStream;
}
