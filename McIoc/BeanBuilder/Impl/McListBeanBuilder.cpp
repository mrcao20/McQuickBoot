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
#include "McListBeanBuilder.h"

MC_DECL_PRIVATE_DATA(McListBeanBuilder)
QVariantList values;
MC_DECL_PRIVATE_DATA_END

McListBeanBuilder::McListBeanBuilder() noexcept
{
    MC_NEW_PRIVATE_DATA(McListBeanBuilder);
}

McListBeanBuilder::~McListBeanBuilder() {}

void McListBeanBuilder::addValue(const QVariant &var) noexcept
{
    d->values.append(var);
}

void McListBeanBuilder::setValues(const QVariantList &vars) noexcept
{
    d->values = vars;
}

bool McListBeanBuilder::isPointer() const noexcept
{
    return false;
}

QVariant McListBeanBuilder::create() noexcept
{
    auto bean = QVariant(QVariantList());
    return bean;
}

void McListBeanBuilder::complete(QVariant &bean, QThread *thread) noexcept
{
    Q_UNUSED(thread)
    bean.setValue(convert(QVariant(d->values), QVariant()));
}

void McListBeanBuilder::doMoveToThread(const QVariant &bean, QThread *thread, const QVariantHash &properties) noexcept
{
    Q_UNUSED(bean)
    Q_UNUSED(thread)
    Q_UNUSED(properties)
}
