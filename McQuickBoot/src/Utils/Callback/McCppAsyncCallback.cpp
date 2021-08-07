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
#include "McBoot/Utils/Callback/Impl/McCppAsyncCallback.h"

#include "McBoot/McBootGlobal.h"
#include "McBoot/Utils/McJsonUtils.h"

MC_STATIC()
qRegisterMetaType<McCppAsyncCallback>();
MC_REGISTER_BEAN_FACTORY(McCppAsyncCallback);
MC_STATIC_END

MC_DECL_PRIVATE_DATA(McCppAsyncCallback)
QList<bool> isQVariants;
QList<int> argumentIds;
const QObject *receiver{nullptr};
QtPrivate::QSlotObjectBase *callback{nullptr};

void deinit()
{
    if (callback == nullptr) {
        return;
    }
    callback->destroyIfLastRef();
    callback = nullptr;
}
MC_PRIVATE_DATA_DESTRUCTOR(McCppAsyncCallback)
deinit();
MC_PRIVATE_DATA_DESTRUCTOR_END
MC_DECL_PRIVATE_DATA_END

McCppAsyncCallback::McCppAsyncCallback() noexcept
{
    MC_NEW_PRIVATE_DATA(McCppAsyncCallback);
}

McCppAsyncCallback::~McCppAsyncCallback() {}

McCppAsyncCallback::McCppAsyncCallback(const McCppAsyncCallback &o) noexcept : McCppAsyncCallback()
{
    d->isQVariants = o.d->isQVariants;
    d->argumentIds = o.d->argumentIds;
    d->receiver = o.d->receiver;
    d->callback = o.d->callback;
    d->callback->ref();
}

McCppAsyncCallback::McCppAsyncCallback(McCppAsyncCallback &&o) noexcept
{
    d.swap(o.d);
}

McCppAsyncCallback &McCppAsyncCallback::operator=(const McCppAsyncCallback &o) noexcept
{
    d->deinit();
    d->isQVariants = o.d->isQVariants;
    d->argumentIds = o.d->argumentIds;
    d->receiver = o.d->receiver;
    d->callback = o.d->callback;
    d->callback->ref();
    return *this;
}

McCppAsyncCallback &McCppAsyncCallback::operator=(McCppAsyncCallback &&o) noexcept
{
    d.swap(o.d);
    return *this;
}

void McCppAsyncCallback::call(const QVariantList &varList) noexcept
{
    QVariantList argList = varList;
    if (argList.length() < d->isQVariants.length()) {
        qCCritical(mcQuickBoot(),
                   "McCppAsyncCallback::call. receiver argument count must be equal to or less "
                   "than sender.");
        return;
    }
    void **args = new void *[argList.length() + 1];
    args[0] = nullptr;
    for (int i = 0; i < argList.length(); ++i) {
        QVariant &var = argList[i];
        if (i >= d->isQVariants.length()) {
            args[i + 1] = const_cast<void *>(var.data());
        } else if (d->isQVariants.at(i)) {
            var = McJsonUtils::serialize(var);
            args[i + 1] = const_cast<void *>((const void *) &var);
        } else {
            auto id = d->argumentIds.at(i);
            if (var.userType() != id) {
                var = McJsonUtils::serialize(var);
                if (var.userType() == qMetaTypeId<QJsonObject>()) {
                    var = McJsonUtils::deserialize(var, id);
                }
                if (var.userType() != id) {
                    qCCritical(mcQuickBoot(),
                               "cannot construct object for typeId: %d. className: %s",
                               id,
                               QMetaType::typeName(id));
                    return;
                }
            }
            args[i + 1] = const_cast<void *>(var.data());
        }
    }
    d->callback->call(const_cast<QObject *>(d->receiver), args);
    delete[] args;
}

void McCppAsyncCallback::init(const QList<bool> &isQVariants,
                              const QList<int> &argumentIds,
                              const QObject *receiver,
                              QtPrivate::QSlotObjectBase *callback) noexcept
{
    d->isQVariants = isQVariants;
    d->argumentIds = argumentIds;
    d->receiver = receiver;
    d->callback = callback;
}
