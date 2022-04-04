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
#include "McObjectClassBeanBuilder.h"

MC_DECL_PRIVATE_DATA(McObjectClassBeanBuilder)
McMetaType metaType;
MC_DECL_PRIVATE_DATA_END

McObjectClassBeanBuilder::McObjectClassBeanBuilder() noexcept
{
    MC_NEW_PRIVATE_DATA(McObjectClassBeanBuilder);
}

McObjectClassBeanBuilder::~McObjectClassBeanBuilder() {}

McMetaType McObjectClassBeanBuilder::metaType() const noexcept
{
    return d->metaType;
}

void McObjectClassBeanBuilder::setMetaType(const McMetaType &type) noexcept
{
    d->metaType = type;
}

void McObjectClassBeanBuilder::setClassName(const QByteArray &className) noexcept
{
    McMetaType type = McMetaType::fromTypeName(className);
    if (Q_UNLIKELY(!type.isValid())) {
        qCCritical(mcIoc(), "class '%s' is not registered", className.constData());
        return;
    }
    setMetaType(type);
}

bool McObjectClassBeanBuilder::isPointer() const noexcept
{
    return true;
}

QVariant McObjectClassBeanBuilder::create() noexcept
{
    if (Q_UNLIKELY(!d->metaType.isValid())) {
        return QVariant();
    }
    auto beanStar = d->metaType.metaType().create();
    if (Q_UNLIKELY(beanStar == nullptr)) {
        qCCritical(mcIoc(), "cannot create object: '%s'", d->metaType.metaType().name());
        return QVariant();
    }
    QVariant beanVar(d->metaType.pMetaType(), &beanStar);
    return beanVar;
}
