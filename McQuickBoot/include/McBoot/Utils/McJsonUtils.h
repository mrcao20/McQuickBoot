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

#include <QList>
#include <QMap>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>

#include "../McBootGlobal.h"

/*!
 * \brief The McJsonUtils class
 * 
 * QObject转QJsonObject的工具类，只能转简单对象
 * 比如A类中包含一个B类，B是直接继承至QObject。
 * @note 如果类C继承至QObject，同时实现至接口I，而类A中包含接口I，则无法转换，
 * 但是包含类C或QObject则可转换
 */
class MCQUICKBOOT_EXPORT McJsonUtils 
{
public:
    static QJsonValue toJson(const QVariant &var) noexcept;
    static QJsonObject toJson(QObject *obj) noexcept;
    static QJsonObject toJson(QObjectConstPtrRef obj) noexcept;
    template<typename T>
    static QJsonObject toJson(T *obj) noexcept;
    template<typename T>
    static QJsonObject toJson(const QSharedPointer<T> &obj) noexcept;
    template<typename T>
    static QJsonArray toJson(const QList<QSharedPointer<T>> &objs) noexcept;
    template<typename T>
    static QJsonArray toJson(const QList<T *> &objs) noexcept;
    static QJsonArray toJson(const QVariantList &vars) noexcept;
    static QJsonObject toJson(const QMap<QString, QObject *> &objs) noexcept;
    static QJsonObject toJson(const QMap<QString, QObjectPtr> &objs) noexcept;
    static QJsonObject toJson(const QVariantMap &objs) noexcept;
    template<typename T>
    static QJsonObject toJsonOnObject(const T &obj) noexcept;
    template<typename T>
    static QJsonArray toJsonOnObject(const QList<T> &objs) noexcept;
#if QT_VERSION >= QT_VERSION_CHECK(5, 6, 0)
    static QJsonObject toJson(const void *gadget, const QMetaObject *mobj) noexcept;
    template<typename T>
    static QJsonObject toJsonOnGadget(const T &gadget) noexcept;
    template<typename T>
    static QJsonArray toJsonOnGadget(const QList<T> &gadgets) noexcept;
#endif

    static QVariant fromJson(const QByteArray &typeName, const QVariant &value) noexcept;
    static QVariant fromJson(int type, const QVariant &value) noexcept;
    template<typename T>
    static T fromJson(const QVariant &value) noexcept;
    template<typename T>
    static T fromJson(const QJsonObject &value) noexcept;
    template<typename T>
    static T fromJson(const QJsonArray &value) noexcept;

    static QVariant serialize(const QVariant &origin) noexcept;
    static QVariant deserialize(const QVariant &origin, int toId) noexcept;
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
    if constexpr (QtPrivate::IsSharedPointerToTypeDerivedFromQObject<QSharedPointer<T>>::Value) {
        return McJsonUtils::toJson(obj.template objectCast<QObject>());
    } else {
        return McJsonUtils::toJson(obj.data(), &T::staticMetaObject);
    }
}

template<typename T>
QJsonArray McJsonUtils::toJson(const QList<QSharedPointer<T>> &objs) noexcept 
{
    QJsonArray result;
    for(auto obj : objs) {
        result.append(QJsonValue(McJsonUtils::toJson(obj)));
    }
    return result;
}

template<typename T>
QJsonArray McJsonUtils::toJson(const QList<T *> &objs) noexcept 
{
    QJsonArray result;
    for (auto obj : objs) {
        result.append(QJsonValue(McJsonUtils::toJson(obj)));
    }
    return result;
}

template<typename T>
QJsonObject McJsonUtils::toJsonOnObject(const T &obj) noexcept
{
    return McJsonUtils::toJson(&obj);
}

template<typename T>
QJsonArray McJsonUtils::toJsonOnObject(const QList<T> &objs) noexcept
{
    QJsonArray result;
    for (auto obj : objs) {
        result.append(QJsonValue(McJsonUtils::toJson(&obj)));
    }
    return result;
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
    for (auto gadget : gadgets) {
        result.append(QJsonValue(McJsonUtils::toJsonOnGadget(gadget)));
    }
    return result;
}

template<typename T>
T McJsonUtils::fromJson(const QVariant &value) noexcept
{
    return McJsonUtils::fromJson(qMetaTypeId<T>(), value).template value<T>();
}

template<typename T>
T McJsonUtils::fromJson(const QJsonObject &value) noexcept
{
    return McJsonUtils::fromJson<T>(QVariant(value));
}

template<typename T>
T McJsonUtils::fromJson(const QJsonArray &value) noexcept
{
    Q_STATIC_ASSERT(QtPrivate::IsSequentialContainer<T>::Value);
    typedef typename T::value_type ValueType;
    T list;
    for (auto jsonValue : value) {
        list.append(McJsonUtils::fromJson<ValueType>(jsonValue.toVariant()));
    }
    return list;
}
