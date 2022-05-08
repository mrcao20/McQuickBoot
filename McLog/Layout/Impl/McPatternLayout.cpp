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
#include "McPatternLayout.h"

#include "Utils/McMessagePattern.h"

MC_AUTO_INIT(McPatternLayout)
MC_INIT_END

MC_DECL_PRIVATE_DATA(McPatternLayout)
QString pattern;
McPrivate::MessagePatternPtr messagePattern;
MC_DECL_PRIVATE_DATA_END

McPatternLayout::McPatternLayout() noexcept
{
    MC_NEW_PRIVATE_DATA(McPatternLayout);

    d->messagePattern = McPrivate::MessagePatternPtr::create();
}

McPatternLayout::~McPatternLayout() {}

QString McPatternLayout::pattern() const noexcept
{
    return d->pattern;
}

void McPatternLayout::setPattern(const QString &val) noexcept
{
    d->pattern = val;

    d->messagePattern->setPattern(val);
}

QString McPatternLayout::format(QtMsgType type, const QMessageLogContext &context, const QString &str) noexcept
{
    return McPrivate::format(d->messagePattern, type, context, str);
}
