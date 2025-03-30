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
