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
#include "McSimpleBeanBuilder.h"

MC_DECL_PRIVATE_DATA(McSimpleBeanBuilder)
QVariant bean;
bool isPointer{false};
MC_DECL_PRIVATE_DATA_END

McSimpleBeanBuilder::McSimpleBeanBuilder(const QVariant &bean) noexcept
{
    MC_NEW_PRIVATE_DATA(McSimpleBeanBuilder);

    d->bean = bean;
}

McSimpleBeanBuilder::~McSimpleBeanBuilder() {}

void McSimpleBeanBuilder::setPointer(bool val) noexcept
{
    d->isPointer = val;
}

QVariant McSimpleBeanBuilder::build(QThread *thread) noexcept
{
    Q_UNUSED(thread)
    return d->bean;
}

void McSimpleBeanBuilder::moveToThread(QThread *thread) noexcept
{
    Q_UNUSED(thread)
}

bool McSimpleBeanBuilder::isSingleton() const noexcept
{
    return true;
}

bool McSimpleBeanBuilder::isPointer() const noexcept
{
    return d->isPointer;
}

void McSimpleBeanBuilder::setReferenceResolver(IMcBeanReferenceResolver *resolver) noexcept
{
    Q_UNUSED(resolver)
}
