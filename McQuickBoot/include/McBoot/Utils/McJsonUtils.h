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
    static QJsonObject toJson(void *gadget, const QMetaObject *mobj) noexcept;
#endif

    static void fromJson(const QJsonObject &jsonObj, QObject *obj) noexcept;
    static void fromJson(const QJsonArray &jsonArr, const QList<QObject *> &objs) noexcept;
    template<typename T>
    struct QObjectFromJson
    {
        using Type = T;
    };

    template<typename T>
    static typename QObjectFromJson<T>::Type fromJson(const QJsonObject &jsonObj) noexcept;
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
