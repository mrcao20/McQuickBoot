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
#pragma once

#include <QElapsedTimer>

#include "../McLogGlobal.h"

namespace McPrivate {
/*!
 * \see qSetMessagePattern
 */
struct MessagePattern
{
    MessagePattern() noexcept;
    ~MessagePattern();

    void setPattern(const QString &pattern) noexcept;

    std::unique_ptr<std::unique_ptr<const char[]>[]> literals;
    std::unique_ptr<const char *[]> tokens;
    QList<QString> timeArgs;
    QElapsedTimer timer;
};
MC_DECL_POINTER(MessagePattern)

QString format(
    const MessagePatternPtr &pattern, QtMsgType type, const QMessageLogContext &context, const QString &str) noexcept;
} // namespace McPrivate
