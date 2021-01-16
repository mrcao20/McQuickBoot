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
    static QJsonArray toJson(const QList<QSharedPointer<T>> &objs) noexcept;
    template<typename T>
    static QJsonArray toJson(const QList<T *> &objs) noexcept;
    static QJsonArray toJson(const QVariantList &vars) noexcept;
    static QJsonObject toJson(const QMap<QString, QObject *> &objs) noexcept;
    static QJsonObject toJson(const QMap<QString, QObjectPtr> &objs) noexcept;
    static QJsonObject toJson(const QVariantMap &objs) noexcept;
#if QT_VERSION >= QT_VERSION_CHECK(5, 6, 0)
    static QJsonObject toJson(const void *gadget, const QMetaObject *mobj) noexcept;
#endif

    static QVariant fromJson(const QByteArray &typeName, const QVariant &value) noexcept;
    static QVariant fromJson(int type, const QVariant &value) noexcept;
    template<typename T>
    static T fromJson(const QVariant &value) noexcept;
    template<typename T>
    static T fromJson(const QJsonObject &value) noexcept;
    template<typename T>
    static T fromJson(const QJsonArray &value) noexcept;
};

template<typename T>
QJsonArray McJsonUtils::toJson(const QList<QSharedPointer<T>> &objs) noexcept 
{
    QList<QObjectPtr> results;
    for(auto obj : objs) {
        results.append(obj);
    }
    QJsonArray result;
    for(auto obj : results) {
        result.append(QJsonValue(McJsonUtils::toJson(obj)));
    }
    return result;
}

template<typename T>
QJsonArray McJsonUtils::toJson(const QList<T *> &objs) noexcept 
{
    QList<QObject *> results;
    for(auto obj : objs) {
        results.append(obj);
    }
    QJsonArray result;
    for(auto obj : results) {
        result.append(QJsonValue(McJsonUtils::toJson(obj)));
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
