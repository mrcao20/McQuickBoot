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
#include "McSlotObjectWrapper.h"

#include "Utils/McJsonUtils.h"

MC_DECL_PRIVATE_DATA(McSlotObjectWrapper)
QMetaType qmetaType;
const QObject *recever{nullptr};
QtPrivate::QSlotObjectBase *method{nullptr};
MC_DECL_PRIVATE_DATA_END

McSlotObjectWrapper::McSlotObjectWrapper(
    const QMetaType &qmetaType, const QObject *recever, QtPrivate::QSlotObjectBase *method) noexcept
{
    d = std::make_unique<MC_PRIVATE_DATA_NAME(McSlotObjectWrapper)>();

    d->qmetaType = qmetaType;
    d->recever = recever;
    d->method = method;
}

McSlotObjectWrapper::~McSlotObjectWrapper()
{
    if (d->method != nullptr) {
        d->method->destroyIfLastRef();
        d->method = nullptr;
    }
}

McSlotObjectWrapper::McSlotObjectWrapper(const McSlotObjectWrapper &o) noexcept
    : McSlotObjectWrapper(o.d->qmetaType, o.d->recever, o.d->method)
{
    if (d->method != nullptr) {
        d->method->ref();
    }
}

McSlotObjectWrapper &McSlotObjectWrapper::operator=(const McSlotObjectWrapper &o) noexcept
{
    McSlotObjectWrapper copy(o);
    d.swap(copy.d);
    return *this;
}

McSlotObjectWrapper::McSlotObjectWrapper(McSlotObjectWrapper &&o) noexcept
    : McSlotObjectWrapper(o.d->qmetaType, o.d->recever, o.d->method)
{
    o.d->method = nullptr;
}

McSlotObjectWrapper &McSlotObjectWrapper::operator=(McSlotObjectWrapper &&o) noexcept
{
    McSlotObjectWrapper copy(std::move(o));
    d.swap(copy.d);
    return *this;
}

const QObject *McSlotObjectWrapper::recever() const noexcept
{
    return d->recever;
}

void McSlotObjectWrapper::call(const QVariant &var) const noexcept
{
    if (d->method == nullptr) {
        return;
    }
    auto body = var;
    void *bodyStar = nullptr;
    if (d->qmetaType == QMetaType::fromType<QVariant>()) {
        body = McJsonUtils::serialize(body);
        bodyStar = &body;
    } else if (d->qmetaType.isValid()) {
        if (body.metaType() != d->qmetaType) {
            body = McJsonUtils::serialize(body);
            if (body.metaType() == QMetaType::fromType<QJsonObject>()) {
                body = McJsonUtils::deserialize(body, d->qmetaType);
            }
            if (body.metaType() != d->qmetaType) {
                qCCritical(mcCore(), "cannot construct object for className: %s", d->qmetaType.name());
                return;
            }
        }
        bodyStar = body.data();
    }
    void *args[] = {nullptr, bodyStar};
    d->method->call(const_cast<QObject *>(d->recever), args);
}
