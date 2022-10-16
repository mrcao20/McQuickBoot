/*
 * Copyright (c) 2021 mrcao20/mrcao20@163.com
 * McQuickBoot is licensed under Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at:
 *          http://license.coscl.org.cn/MulanPSL2
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
 * EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
 * MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
 * See the Mulan PSL v2 for more details.
 */
#include "McSlotObjectWrapper.h"

#include <QPointer>
#include <QScopeGuard>

#include "McJsonUtils.h"

MC_DECL_PRIVATE_DATA(McSlotObjectWrapper)
bool hasRecever{false};
QPointer<const QObject> recever;
#ifdef MC_USE_QT5
QList<int> qmetaTypes;
#else
QList<QMetaType> qmetaTypes;
#endif
QtPrivate::QSlotObjectBase *method{nullptr};
MC_DECL_PRIVATE_DATA_END

#ifdef MC_USE_QT5
McSlotObjectWrapper::McSlotObjectWrapper(
    const QObject *recever, const QList<int> &qmetaTypes, QtPrivate::QSlotObjectBase *method) noexcept
#else
McSlotObjectWrapper::McSlotObjectWrapper(
    const QObject *recever, const QList<QMetaType> &qmetaTypes, QtPrivate::QSlotObjectBase *method) noexcept
#endif
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
#ifdef MC_USE_QT5
        } else if (d->qmetaTypes.at(i) == qMetaTypeId<QVariant>()) {
#else
        } else if (d->qmetaTypes.at(i) == QMetaType::fromType<QVariant>()) {
#endif
            body = McJsonUtils::serialize(body);
            args[i + 1] = const_cast<void *>((const void *)&body);
#ifdef MC_USE_QT5
        } else if (d->qmetaTypes.at(i) != -1) {
#else
        } else if (d->qmetaTypes.at(i).isValid()) {
#endif
            auto qmetaType = d->qmetaTypes.at(i);
#ifdef MC_USE_QT5
            if (body.userType() != qmetaType) {
                qCCritical(mcCore, "cannot construct object for className: %s", QMetaType::typeName(qmetaType));
                return;
            }
#else
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
#endif
            args[i + 1] = const_cast<void *>(body.constData());
        }
    }
    d->method->call(const_cast<QObject *>(d->recever.data()), args);
}
