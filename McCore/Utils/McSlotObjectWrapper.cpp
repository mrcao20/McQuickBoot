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
bool hasRecever{false};
QPointer<const QObject> recever;
QList<QMetaType> qmetaTypes;
QtPrivate::QSlotObjectBase *method{nullptr};
MC_DECL_PRIVATE_DATA_END

McSlotObjectWrapper::McSlotObjectWrapper(
    const QObject *recever, const QList<QMetaType> &qmetaTypes, QtPrivate::QSlotObjectBase *method) noexcept
{
    MC_NEW_PRIVATE_DATA(McSlotObjectWrapper);

    if (recever != nullptr) {
        d->hasRecever = true;
    } else {
        d->hasRecever = false;
    }
    d->recever = recever;
    d->qmetaTypes = qmetaTypes;
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
    : McSlotObjectWrapper(o.d->recever.data(), o.d->qmetaTypes, o.d->method)
{
    d->hasRecever = o.d->hasRecever;
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
    : McSlotObjectWrapper(o.d->recever.data(), o.d->qmetaTypes, o.d->method)
{
    d->hasRecever = o.d->hasRecever;
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
    return d->recever.data();
}

void McSlotObjectWrapper::call(const QVariantList &varList) const noexcept
{
    if ((d->hasRecever && d->recever.isNull()) || d->method == nullptr) {
        return;
    }
    auto argList = varList;
    if (argList.length() < d->qmetaTypes.length()) {
        qCCritical(
            mcCore(), "McSlotObjectWrapper::call. receiver argument count must be equal to or less than sender.");
        return;
    }
    void **args = new void *[argList.length() + 1];
    auto cleanup = qScopeGuard([&args]() { delete[] args; });
    args[0] = nullptr;
    for (int i = 0; i < argList.length(); ++i) {
        QVariant &body = argList[i];
        if (i >= d->qmetaTypes.length()) {
            args[i + 1] = const_cast<void *>(body.constData());
        } else if (d->qmetaTypes.at(i) == QMetaType::fromType<QVariant>()) {
            body = McJsonUtils::serialize(body);
            args[i + 1] = const_cast<void *>((const void *)&body);
        } else if (d->qmetaTypes.at(i).isValid()) {
            auto qmetaType = d->qmetaTypes.at(i);
            if (body.metaType() != qmetaType) {
                body = McJsonUtils::serialize(body);
                if (body.metaType() == QMetaType::fromType<QJsonObject>()) {
                    body = McJsonUtils::deserialize(body, qmetaType);
                }
                if (body.metaType() != qmetaType) {
                    qCCritical(mcCore(), "cannot construct object for className: %s", qmetaType.name());
                    return;
                }
            }
            args[i + 1] = const_cast<void *>(body.constData());
        }
    }
    d->method->call(const_cast<QObject *>(d->recever.data()), args);
}
