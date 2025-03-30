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

MC_DECL_PRIVATE_DATA2(McSlotObjectWrapper)
{
    bool hasReceiver{false};
    QPointer<const QObject> receiver;
#ifdef MC_USE_QT5
    QList<int> qmetaTypes;
    int returnMetaType;
#else
    QList<QMetaType> qmetaTypes;
    QMetaType returnMetaType;
#endif
    bool isForceReturnQVariant{false};
    QtPrivate::QSlotObjectBase *method{nullptr};
};

McSlotObjectWrapper::McSlotObjectWrapper() noexcept
{
    MC_NEW_PRIVATE_DATA(McSlotObjectWrapper);
}

#ifdef MC_USE_QT5
McSlotObjectWrapper::McSlotObjectWrapper(const QObject *receiver, const QList<int> &qmetaTypes, int returnMetaType,
    QtPrivate::QSlotObjectBase *method) noexcept
#else
McSlotObjectWrapper::McSlotObjectWrapper(const QObject *receiver, const QList<QMetaType> &qmetaTypes,
    QMetaType returnMetaType, QtPrivate::QSlotObjectBase *method) noexcept
#endif
    : McSlotObjectWrapper()
{
    if (receiver != nullptr) {
        d->hasReceiver = true;
    } else {
        d->hasReceiver = false;
    }
    d->receiver = receiver;
    d->qmetaTypes = qmetaTypes;
    d->returnMetaType = returnMetaType;
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
    : McSlotObjectWrapper(o.d->receiver.data(), o.d->qmetaTypes, o.d->returnMetaType, o.d->method)
{
    d->hasReceiver = o.d->hasReceiver;
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
    : McSlotObjectWrapper(o.d->receiver.data(), o.d->qmetaTypes, o.d->returnMetaType, o.d->method)
{
    d->hasReceiver = o.d->hasReceiver;
    o.d->method = nullptr;
}

McSlotObjectWrapper &McSlotObjectWrapper::operator=(McSlotObjectWrapper &&o) noexcept
{
    McSlotObjectWrapper copy(std::move(o));
    d.swap(copy.d);
    return *this;
}

const QObject *McSlotObjectWrapper::receiver() const noexcept
{
    return d->receiver.data();
}

#ifdef MC_USE_QT5
QList<int> McSlotObjectWrapper::metaTypes() const noexcept
#else
QList<QMetaType> McSlotObjectWrapper::metaTypes() const noexcept
#endif
{
    return d->qmetaTypes;
}

#ifdef MC_USE_QT5
int McSlotObjectWrapper::returnMetaType() const noexcept
#else
QMetaType McSlotObjectWrapper::returnMetaType() const noexcept
#endif
{
    return d->returnMetaType;
}

void McSlotObjectWrapper::setForceReturnQVariant(bool val) noexcept
{
    d->isForceReturnQVariant = val;
}

QVariant McSlotObjectWrapper::call(const QVariantList &varList) const
{
    if ((d->hasReceiver && d->receiver.isNull()) || d->method == nullptr) {
        return QVariant();
    }
    auto argList = varList;
    if (argList.length() < d->qmetaTypes.length()) {
        qCCritical(
            mcCore(), "McSlotObjectWrapper::call. receiver argument count must be equal to or less than sender.");
        return QVariant();
    }
    void **args = new void *[argList.length() + 1];
    auto cleanup = qScopeGuard([&args]() { delete[] args; });
    QVariant returnValue;
    if (d->isForceReturnQVariant) {
        args[0] = &returnValue;
    } else {
#ifdef MC_USE_QT5
        if (d->returnMetaType != qMetaTypeId<void>()) {
            returnValue = QVariant(static_cast<QVariant::Type>(d->returnMetaType));
#else
        if (d->returnMetaType != QMetaType::fromType<void>()) {
            returnValue = QVariant(d->returnMetaType);
#endif
        }
        args[0] = const_cast<void *>(returnValue.constData());
    }
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
                body = McJsonUtils::serialize(body);
                if (body.userType() == qMetaTypeId<QJsonObject>()) {
                    body = McJsonUtils::deserialize(body, qmetaType);
                }
                if (body.userType() != qmetaType && !body.convert(qmetaType)) {
                    qCCritical(mcCore, "cannot construct object for className: %s", QMetaType::typeName(qmetaType));
                    return QVariant();
                }
            }
#else
            if (body.metaType() != qmetaType) {
                body = McJsonUtils::serialize(body);
                if (body.metaType() == QMetaType::fromType<QJsonObject>()) {
                    body = McJsonUtils::deserialize(body, qmetaType);
                }
                if (body.metaType() != qmetaType && !body.convert(qmetaType)) {
                    qCCritical(mcCore(), "cannot construct object for className: %s", qmetaType.name());
                    return QVariant();
                }
            }
#endif
            args[i + 1] = const_cast<void *>(body.constData());
        }
    }
    d->method->call(const_cast<QObject *>(d->receiver.data()), args);
    return returnValue;
}

void McSlotObjectWrapper::call(void **arguments) const
{
    if ((d->hasReceiver && d->receiver.isNull()) || d->method == nullptr) {
        return;
    }
    d->method->call(const_cast<QObject *>(d->receiver.data()), arguments);
}
