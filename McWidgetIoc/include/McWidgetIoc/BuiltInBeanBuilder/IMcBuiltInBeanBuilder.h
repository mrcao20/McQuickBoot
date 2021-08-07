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

#include <McIoc/McGlobal.h>

QT_BEGIN_NAMESPACE
class QWidget;
QT_END_NAMESPACE

class IMcBuiltInBeanBuilder
{
public:
    virtual ~IMcBuiltInBeanBuilder() = default;

    /*!
     * \brief build
     * 构造对象
     */
    virtual QWidget *build() noexcept = 0;
    /*!
     * \brief setProperty
     * 对已经构造出的对象设置参数
     * \note 调用此函数前会先调用build
     * \see build
     */
    virtual void setProperty(const QString &name, const QVariant &val) noexcept = 0;
};

MC_DECL_POINTER(IMcBuiltInBeanBuilder)
