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

#include "../McLogGlobal.h"

#ifdef MC_USE_QT5
QT_BEGIN_NAMESPACE
class QTextCodec;
QT_END_NAMESPACE
#else
# include <QStringConverter>
#endif

class IMcCodecableAppender
{
    MC_DEFINE_INTERFACE(IMcCodecableAppender)
public:
#ifdef MC_USE_QT5
    virtual QTextCodec *codec() const noexcept = 0;
    virtual void setCodec(QTextCodec *val) noexcept = 0;
#else
    virtual QStringConverter::Encoding encoding() const noexcept = 0;
    virtual void setEncoding(QStringConverter::Encoding val) noexcept = 0;
#endif
};

MC_DECL_METATYPE(IMcCodecableAppender)
