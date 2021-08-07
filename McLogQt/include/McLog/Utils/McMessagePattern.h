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

struct McMessagePattern 
{
    McMessagePattern();
    ~McMessagePattern();

    void setPattern(const QString &pattern);

    // 0 terminated arrays of literal tokens / literal or placeholder tokens
    const char **literals;
    const char **tokens;
    QList<QString> timeArgs;   // timeFormats in sequence of %{time
#ifndef QT_BOOTSTRAPPED
    QElapsedTimer timer;
#endif
#ifdef QLOGGING_HAVE_BACKTRACE
    struct BacktraceParams 
    {
        QString backtraceSeparator;
        int backtraceDepth;
    };
    QVector<BacktraceParams> backtraceArgs; // backtrace argumens in sequence of %{backtrace
#endif
};
MC_DECL_POINTER(McMessagePattern)
#ifdef QLOGGING_HAVE_BACKTRACE
Q_DECLARE_TYPEINFO(McMessagePattern::BacktraceParams, Q_MOVABLE_TYPE);
#endif

QString format(McMessagePatternPtr pattern, QtMsgType type, const QMessageLogContext &context, const QString &str) noexcept;

}
