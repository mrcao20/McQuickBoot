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
#include "McAbstractIODeviceAppender.h"

#include <QIODevice>
#include <QTextStream>

MC_DECL_PRIVATE_DATA(McAbstractIODeviceAppender)
QIODevicePtr device;
QTextStream textStream;
QStringConverter::Encoding encoding{QStringConverter::Utf8};
MC_DECL_PRIVATE_DATA_END

McAbstractIODeviceAppender::McAbstractIODeviceAppender() noexcept
{
    MC_NEW_PRIVATE_DATA(McAbstractIODeviceAppender);
}

McAbstractIODeviceAppender::~McAbstractIODeviceAppender()
{
    if (!d->device.isNull() && d->device->isOpen())
        d->device->close();
}

QIODevicePtr McAbstractIODeviceAppender::device() const noexcept
{
    return d->device;
}

void McAbstractIODeviceAppender::setDevice(const QIODevicePtr &device) noexcept
{
    d->device = device;
}

QByteArray McAbstractIODeviceAppender::codecName() const noexcept
{
    return QStringConverter::nameForEncoding(d->encoding);
}

void McAbstractIODeviceAppender::setCodecName(const QByteArray &val) noexcept
{
    auto tmp = QStringConverter::encodingForName(val.constData());
    if (!tmp.has_value()) {
        return;
    }
    d->encoding = *tmp;
}

QStringConverter::Encoding McAbstractIODeviceAppender::encoding() const noexcept
{
    return d->encoding;
}

void McAbstractIODeviceAppender::setEncoding(QStringConverter::Encoding val) noexcept
{
    d->encoding = val;
}

void McAbstractIODeviceAppender::buildCompleted() noexcept
{
    super::buildCompleted();

    d->textStream.setDevice(d->device.data());
    d->textStream.setEncoding(d->encoding);
}

QTextStream &McAbstractIODeviceAppender::textStream() noexcept
{
    return d->textStream;
}
