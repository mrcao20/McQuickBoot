/*
 * Copyright (c) 2021 mrcao20/mrcao20@163.com
 * McQuickBoot is licensed under Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at:
 *          http://license.coscl.org.cn/MulanPSL2
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
 * EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
 * MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
 * See the Mulan PSL v2 for more details.
 */
#include "McAbstractIODeviceAppender.h"

#include <QIODevice>
#include <QTextStream>
#ifdef MC_USE_QT5
# include <QTextCodec>
#endif

MC_DECL_PRIVATE_DATA(McAbstractIODeviceAppender)
QIODevicePtr device;
QTextStream textStream;
#ifdef MC_USE_QT5
QByteArray codecName{"UTF-8"};
QTextCodec *codec{nullptr};
#else
QStringConverter::Encoding encoding{QStringConverter::Utf8};
#endif
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
#ifdef MC_USE_QT5
    return d->codecName;
#else
    return QStringConverter::nameForEncoding(d->encoding);
#endif
}

void McAbstractIODeviceAppender::setCodecName(const QByteArray &val) noexcept
{
#ifdef MC_USE_QT5
    d->codecName = val;
#else
    auto tmp = QStringConverter::encodingForName(val.constData());
    if (!tmp.has_value()) {
        return;
    }
    d->encoding = *tmp;
#endif
}

#ifdef MC_USE_QT5
QTextCodec *McAbstractIODeviceAppender::codec() const noexcept
{
    return d->codec;
}

void McAbstractIODeviceAppender::setCodec(QTextCodec *val) noexcept
{
    d->codec = val;
}
#else
QStringConverter::Encoding McAbstractIODeviceAppender::encoding() const noexcept
{
    return d->encoding;
}

void McAbstractIODeviceAppender::setEncoding(QStringConverter::Encoding val) noexcept
{
    d->encoding = val;
}
#endif

void McAbstractIODeviceAppender::buildCompleted() noexcept
{
    super::buildCompleted();

    d->textStream.setDevice(d->device.data());
#ifdef MC_USE_QT5
    if (d->codec == nullptr) {
        d->codec = QTextCodec::codecForName(d->codecName);
    }
    d->textStream.setCodec(d->codec);
#else
    d->textStream.setEncoding(d->encoding);
#endif
}

QTextStream &McAbstractIODeviceAppender::textStream() noexcept
{
    return d->textStream;
}
