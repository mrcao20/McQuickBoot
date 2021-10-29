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

void McAbstractIODeviceAppender::doAllFinished() noexcept
{
    McAbstractAppender::doAllFinished();

    d->textStream.setDevice(d->device.data());
    if (d->codec == nullptr) {
        d->codec = QTextCodec::codecForName(d->codecName);
    }
    //    d->textStream.setCodec(d->codec);
}

QTextStream &McAbstractIODeviceAppender::textStream() noexcept
{
    return d->textStream;
}
