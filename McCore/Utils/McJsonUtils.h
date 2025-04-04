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
#pragma once

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QList>
#include <QMap>

#include "../McGlobal.h"

class MC_CORE_EXPORT McJsonUtils
{
public:
    static QJsonValue toJson(const QVariant &var) noexcept;
    static QJsonObject toJson(QObject *obj) noexcept;
    static QJsonObject toJson(const QObjectPtr &obj) noexcept { return McJsonUtils::toJson(obj.data()); }
    template<typename T>
    static QJsonObject toJson(T *obj) noexcept;
    template<typename T>
    static QJsonObject toJson(const QSharedPointer<T> &obj) noexcept;

    template<typename T>
    static QJsonArray toJson(const QList<QSharedPointer<T>> &objs) noexcept;
    template<typename T>
    static QJsonArray toJson(const QList<T *> &objs) noexcept;
    static QJsonArray toJson(const QVariantList &vars) noexcept;

    template<typename T>
    static QJsonObject toJson(const QMap<QString, T *> &objs) noexcept;
    template<typename T>
    static QJsonObject toJson(const QMap<QString, QSharedPointer<T>> &objs) noexcept;
    static QJsonObject toJson(const QVariantMap &objs) noexcept;

    template<typename T>
    static QJsonObject toJsonOnObject(const T &obj) noexcept;

    static QJsonObject toJson(const void *gadget, const QMetaObject *mobj) noexcept;
    template<typename T>
    static QJsonObject toJsonOnGadget(const T &gadget) noexcept;
    template<typename T>
    static QJsonArray toJsonOnGadget(const QList<T> &gadgets) noexcept;
    template<typename T>
    static QJsonObject toJsonOnGadget(const QMap<QString, T> &gadgets) noexcept;

    static QVariant fromJson(const QByteArray &typeName, const QVariant &value) noexcept;
#ifdef MC_USE_QT5
    static QVariant fromJson(int qmetaType, const QVariant &value) noexcept;
#else
    static QVariant fromJson(const QMetaType &qmetaType, const QVariant &value) noexcept;
#endif
    template<typename T>
    static T fromJson(const QVariant &value) noexcept;
    template<typename T>
    static T fromJson(const QJsonObject &value) noexcept;
    template<typename T>
    static T fromJson(const QJsonArray &value) noexcept;

    //! 序列化和反序列化只针对对象而言，所以origin存储的类型只能是QObject*、QSharedPointer<QObject>、Gadget*和QSharedPointer<Gadget>
    static QVariant serialize(const QVariant &origin) noexcept;
#ifdef MC_USE_QT5
    static QVariant deserialize(const QVariant &origin, int qmetaType) noexcept;
#else
    static QVariant deserialize(const QVariant &origin, const QMetaType &qmetaType) noexcept;
#endif
};

template<typename T>
QJsonObject McJsonUtils::toJson(T *obj) noexcept
{
    if constexpr (QtPrivate::IsPointerToTypeDerivedFromQObject<T *>::Value) {
        return McJsonUtils::toJson(qobject_cast<QObject *>(obj));
    } else {
        return McJsonUtils::toJson(obj, &T::staticMetaObject);
    }
}

template<typename T>
QJsonObject McJsonUtils::toJson(const QSharedPointer<T> &obj) noexcept
{
    return McJsonUtils::toJson(obj.data());
}

template<typename T>
QJsonArray McJsonUtils::toJson(const QList<QSharedPointer<T>> &objs) noexcept
{
    QJsonArray result;
    for (auto &obj : qAsConst(objs)) {
        result.append(QJsonValue(McJsonUtils::toJson(obj)));
    }
    return result;
}

template<typename T>
QJsonArray McJsonUtils::toJson(const QList<T *> &objs) noexcept
{
    QJsonArray result;
    for (auto &obj : qAsConst(objs)) {
        result.append(QJsonValue(McJsonUtils::toJson(obj)));
    }
    return result;
}

template<typename T>
QJsonObject McJsonUtils::toJson(const QMap<QString, T *> &objs) noexcept
{
    QJsonObject result;
    QMapIterator<QString, T *> itr(objs);
    while (itr.hasNext()) {
        auto item = itr.next();
        result.insert(item.key(), McJsonUtils::toJson(item.value()));
    }
    return result;
}

template<typename T>
QJsonObject McJsonUtils::toJson(const QMap<QString, QSharedPointer<T>> &objs) noexcept
{
    QJsonObject result;
    QMapIterator<QString, QSharedPointer<T>> itr(objs);
    while (itr.hasNext()) {
        auto item = itr.next();
        result.insert(item.key(), McJsonUtils::toJson(item.value()));
    }
    return result;
}

template<typename T>
QJsonObject McJsonUtils::toJsonOnObject(const T &obj) noexcept
{
    return McJsonUtils::toJson(&obj);
}

template<typename T>
QJsonObject McJsonUtils::toJsonOnGadget(const T &gadget) noexcept
{
    return McJsonUtils::toJson(&gadget, &T::staticMetaObject);
}

template<typename T>
QJsonArray McJsonUtils::toJsonOnGadget(const QList<T> &gadgets) noexcept
{
    QJsonArray result;
    for (auto &gadget : gadgets) {
        result.append(QJsonValue(McJsonUtils::toJsonOnGadget(gadget)));
    }
    return result;
}

template<typename T>
QJsonObject McJsonUtils::toJsonOnGadget(const QMap<QString, T> &gadgets) noexcept
{
    QJsonObject result;
    QMapIterator<QString, T> itr(gadgets);
    while (itr.hasNext()) {
        auto item = itr.next();
        result.insert(item.key(), McJsonUtils::toJson(item.value()));
    }
    return result;
}

template<typename T>
T McJsonUtils::fromJson(const QVariant &value) noexcept
{
#ifdef MC_USE_QT5
    return McJsonUtils::fromJson(qMetaTypeId<T>(), value).template value<T>();
#else
    return McJsonUtils::fromJson(QMetaType::fromType<T>(), value).template value<T>();
#endif
}

template<typename T>
T McJsonUtils::fromJson(const QJsonObject &value) noexcept
{
    if constexpr (QtPrivate::IsAssociativeContainer<T>::Value) {
        using ValueType = typename T::mapped_type;
        T map;
        auto keys = value.keys();
        for (auto &key : qAsConst(keys)) {
            map.insert(key, McJsonUtils::fromJson<ValueType>(value.value(key).toVariant()));
        }
        return map;
    } else {
        return McJsonUtils::fromJson<T>(QVariant(value));
    }
}

template<typename T>
T McJsonUtils::fromJson(const QJsonArray &value) noexcept
{
    static_assert(QtPrivate::IsSequentialContainer<T>::Value, "T must be list");
    using ValueType = typename T::value_type;
    T list;
    for (auto jsonValue : value) {
        list.append(McJsonUtils::fromJson<ValueType>(jsonValue.toVariant()));
    }
    return list;
}
