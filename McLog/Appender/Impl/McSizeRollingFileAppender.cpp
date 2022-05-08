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
#include "McSizeRollingFileAppender.h"

#include <QFile>

MC_DECL_PRIVATE_DATA(McSizeRollingFileAppender)
QString maxFileSize;
qint64 maxFileSizeBytes{-1}; //!< 单位: byte，默认-1表示用不创建新文件
MC_DECL_PRIVATE_DATA_END

MC_AUTO_INIT(McSizeRollingFileAppender)
MC_INIT_END

McSizeRollingFileAppender::McSizeRollingFileAppender() noexcept
{
    MC_NEW_PRIVATE_DATA(McSizeRollingFileAppender);
}

McSizeRollingFileAppender::~McSizeRollingFileAppender() {}

QString McSizeRollingFileAppender::maxFileSize() const noexcept
{
    return d->maxFileSize;
}

void McSizeRollingFileAppender::setMaxFileSize(const QString &val) noexcept
{
    d->maxFileSize = val.toUpper();

    auto temp = d->maxFileSize;
    if (temp.endsWith("GB")) {
        temp.remove("GB");
        bool ok = false;
        auto size = temp.toLongLong(&ok);
        if (ok) {
            setMaxFileSizeBytes(size * 1024 * 1024 * 1024);
        }
    } else if (temp.endsWith("MB")) {
        temp.remove("MB");
        bool ok = false;
        auto size = temp.toLongLong(&ok);
        if (ok) {
            setMaxFileSizeBytes(size * 1024 * 1024);
        }
    } else if (temp.endsWith("KB")) {
        temp.remove("KB");
        bool ok = false;
        auto size = temp.toLongLong(&ok);
        if (ok) {
            setMaxFileSizeBytes(size * 1024);
        }
    } else if (temp.endsWith("B")) {
        temp.remove("B");
        bool ok = false;
        auto size = temp.toLongLong(&ok);
        if (ok) {
            setMaxFileSizeBytes(size);
        }
    }
}

qint64 McSizeRollingFileAppender::maxFileSizeBytes() const noexcept
{
    return d->maxFileSizeBytes;
}

void McSizeRollingFileAppender::setMaxFileSizeBytes(qint64 val) noexcept
{
    d->maxFileSizeBytes = val;
}

bool McSizeRollingFileAppender::isRolling() noexcept
{
    if (d->maxFileSizeBytes == -1) {
        return false;
    }
    if (device().isNull()) {
        return false;
    }
    auto file = device().staticCast<QFile>();
    if (!file->isOpen()) {
        return false;
    }
    return file->size() >= d->maxFileSizeBytes;
}
