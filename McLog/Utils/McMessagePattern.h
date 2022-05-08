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
