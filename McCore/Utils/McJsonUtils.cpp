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
#include "McJsonUtils.h"

#include <QMetaClassInfo>

namespace {
struct JsonUtils
{
#ifdef MC_USE_QT5
    QVariant makeValue(int qmetaType, const QVariant &arg) noexcept;
    QVariant makeGadgetValue(int qmetaType, McMetaType metaType, const QVariant &arg) noexcept;
    QVariant makeObjectValue(int qmetaType, McMetaType metaType, const QVariant &arg) noexcept;
    QVariant makePlanValue(int qmetaType, const QVariant &arg) noexcept;
#else
    QVariant makeValue(const QMetaType &qmetaType, const QVariant &arg) noexcept;
    QVariant makeGadgetValue(const QMetaType &qmetaType, McMetaType metaType, const QVariant &arg) noexcept;
    QVariant makeObjectValue(const QMetaType &qmetaType, McMetaType metaType, const QVariant &arg) noexcept;
    QVariant makePlanValue(const QMetaType &qmetaType, const QVariant &arg) noexcept;
#endif
    QVariant makeListValue(McListMetaType metaType, const QVariant &arg) noexcept;
    QVariant makeMapValue(McMapMetaType metaType, const QVariant &arg) noexcept;

    QVariant serialize(const QVariant &origin) noexcept;
    QVariant serialize(QObject *obj, const QVariant &origin) noexcept;
    QVariant serialize(const void *obj, const QMetaObject *metaObj, const QVariant &origin) noexcept;
#ifdef MC_USE_QT5
    QVariant deserialize(const QVariant &origin, int qmetaType) noexcept;
#else
    QVariant deserialize(const QVariant &origin, const QMetaType &qmetaType) noexcept;
#endif

    bool isSerialize(const QMetaObject *metaObj) noexcept;
};

#ifdef MC_USE_QT5
QVariant JsonUtils::makeValue(int qmetaType, const QVariant &arg) noexcept
#else
QVariant JsonUtils::makeValue(const QMetaType &qmetaType, const QVariant &arg) noexcept
#endif
{
    auto metaType = McMetaType::fromFuzzyQMetaType(qmetaType);
    if (metaType.isValid()) {
#ifdef MC_USE_QT5
        auto flags = QMetaType::typeFlags(metaType.pMetaType());
#else
        auto flags = metaType.pMetaType().flags();
#endif
        if (flags.testFlag(QMetaType::PointerToQObject)) {
            return makeObjectValue(qmetaType, metaType, arg);
        } else if (flags.testFlag(QMetaType::PointerToGadget)) {
            return makeGadgetValue(qmetaType, metaType, arg);
        } else {
            return arg;
        }
    }
    auto listMetaType = McListMetaType::fromQMetaType(qmetaType);
    if (listMetaType.isValid()) {
        return makeListValue(listMetaType, arg);
    }
    auto mapMetaType = McMapMetaType::fromQMetaType(qmetaType);
#ifdef MC_USE_QT5
    if (mapMetaType.isValid() && mapMetaType.keyMetaType() == qMetaTypeId<QString>()) {
#else
    if (mapMetaType.isValid() && mapMetaType.keyMetaType() == QMetaType::fromType<QString>()) {
#endif
        return makeMapValue(mapMetaType, arg);
    }
    return makePlanValue(qmetaType, arg);
}

#ifdef MC_USE_QT5
QVariant JsonUtils::makeGadgetValue(int qmetaType, McMetaType metaType, const QVariant &arg) noexcept
{
    auto metaObj = QMetaType::metaObjectForType(metaType.pMetaType());
#else
QVariant JsonUtils::makeGadgetValue(const QMetaType &qmetaType, McMetaType metaType, const QVariant &arg) noexcept
{
    auto metaObj = metaType.pMetaType().metaObject();
#endif
    if (metaObj == nullptr) {
        return QVariant();
    }
    auto gadget = metaType.createPointer();
    auto count = metaObj->propertyCount();
    auto args = arg.toMap();
    for (int i = 0; i < count; ++i) {
        QMetaProperty pro = metaObj->property(i);
        auto name = pro.name();
        if (!args.contains(name)) {
            continue;
        }
        auto value = args[name];
#ifdef MC_USE_QT5
        value = makeValue(pro.userType(), value);
#else
        value = makeValue(pro.metaType(), value);
#endif
        if (!pro.writeOnGadget(gadget, value)) {
            qCCritical(mcCore(), "cannot dynamic write value to property '%s' for class '%s'\n", pro.name(),
                metaObj->className());
        }
    }
    QVariant ret;
    if (metaType.pMetaType() == qmetaType) {
        ret = QVariant(qmetaType, &gadget);
    } else if (metaType.sMetaType() == qmetaType) {
        ret = metaType.createSharedPointer(gadget);
    }
    return ret;
}

#ifdef MC_USE_QT5
QVariant JsonUtils::makeObjectValue(int qmetaType, McMetaType metaType, const QVariant &arg) noexcept
{
    auto metaObj = QMetaType::metaObjectForType(metaType.pMetaType());
#else
QVariant JsonUtils::makeObjectValue(const QMetaType &qmetaType, McMetaType metaType, const QVariant &arg) noexcept
{
    auto metaObj = metaType.pMetaType().metaObject();
#endif
    if (metaObj == nullptr) {
        return QVariant();
    }
    QObject *obj = nullptr;
    QVariant ret;
    if (metaType.pMetaType() == qmetaType) {
        auto bean = metaType.createPointer();
        ret = QVariant(qmetaType, &bean);
        obj = ret.value<QObject *>();
    } else if (metaType.sMetaType() == qmetaType) {
        ret = metaType.createSharedPointer();
        obj = ret.value<QObjectPtr>().data();
    } else {
        return QVariant();
    }
    auto count = metaObj->propertyCount();
    auto args = arg.toMap();
    for (int i = 0; i < count; ++i) {
        QMetaProperty pro = metaObj->property(i);
        auto name = pro.name();
        if (!args.contains(name)) {
            continue;
        }
        auto value = args[name];
#ifdef MC_USE_QT5
        value = makeValue(pro.userType(), value);
#else
        value = makeValue(pro.metaType(), value);
#endif
        if (!pro.write(obj, value))
            qCCritical(mcCore(), "cannot dynamic write value to property '%s' for class '%s'\n", pro.name(),
                metaObj->className());
    }
    return ret;
}

QVariant JsonUtils::makeListValue(McListMetaType metaType, const QVariant &arg) noexcept
{
    QVariantList resList;
    auto list = arg.toList();
    auto valueMetaType = metaType.valueMetaType();
    for (auto &var : qAsConst(list)) {
        resList.append(makeValue(valueMetaType, var));
    }
    return resList;
}

QVariant JsonUtils::makeMapValue(McMapMetaType metaType, const QVariant &arg) noexcept
{
    QVariantMap resMap;
    auto map = arg.toMap();
    auto valueMetaType = metaType.valueMetaType();
    QMapIterator<QString, QVariant> itr(map);
    while (itr.hasNext()) {
        auto item = itr.next();
        resMap.insert(item.key(), makeValue(valueMetaType, item.value()));
    }
    return resMap;
}

#ifdef MC_USE_QT5
QVariant JsonUtils::makePlanValue(int qmetaType, const QVariant &arg) noexcept
{
    if (qmetaType == qMetaTypeId<QJsonObject>() && arg.userType() == qMetaTypeId<QVariantMap>()) {
        return QJsonObject::fromVariantMap(arg.value<QVariantMap>());
    } else if (qmetaType == qMetaTypeId<QJsonArray>() && arg.userType() == qMetaTypeId<QVariantList>()) {
        return QJsonArray::fromVariantList(arg.value<QVariantList>());
    }
#else
QVariant JsonUtils::makePlanValue(const QMetaType &qmetaType, const QVariant &arg) noexcept
{
    if (qmetaType == QMetaType::fromType<QJsonObject>() && arg.metaType() == QMetaType::fromType<QVariantMap>()) {
        return QJsonObject::fromVariantMap(arg.value<QVariantMap>());
    } else if (qmetaType == QMetaType::fromType<QJsonArray>() && arg.metaType() == QMetaType::fromType<QVariantList>()) {
        return QJsonArray::fromVariantList(arg.value<QVariantList>());
    }
#endif
    auto value = arg;
    if (!value.convert(qmetaType)) {
#ifdef MC_USE_QT5
        qCCritical(mcCore()) << "property convert failure. origin type name:" << arg.typeName()
                             << "target typeName:" << QMetaType::typeName(qmetaType);
#else
        qCCritical(mcCore()) << "property convert failure. origin type name:" << arg.metaType().name()
                             << "target typeName:" << qmetaType.name();
#endif
        return arg;
    }
    return value;
}

QVariant JsonUtils::serialize(const QVariant &origin) noexcept
{
#ifdef MC_USE_QT5
    auto qmetaType = origin.userType();
#else
    auto qmetaType = origin.metaType();
#endif
    auto metaType = McMetaType::fromFuzzyQMetaType(qmetaType);
    if (metaType.isValid()) {
#ifdef MC_USE_QT5
        auto flags = QMetaType::typeFlags(metaType.pMetaType());
#else
        auto flags = metaType.pMetaType().flags();
#endif
        if (flags.testFlag(QMetaType::PointerToQObject)) {
            if (metaType.pMetaType() == qmetaType) {
                return serialize(origin.value<QObject *>(), origin);
            } else if (metaType.sMetaType() == qmetaType) {
                return serialize(origin.value<QObjectPtr>().data(), origin);
#ifdef MC_USE_QT6
            } else if (metaType.wMetaType() == qmetaType) {
                return serialize(origin.value<QWeakPointer<QObject>>().toStrongRef().data(), origin);
            } else if (metaType.tMetaType() == qmetaType) {
                return serialize(origin.value<QPointer<QObject>>().data(), origin);
#endif
            }
        } else if (flags.testFlag(QMetaType::PointerToGadget)) {
#ifdef MC_USE_QT5
            auto metaObj = QMetaType::metaObjectForType(metaType.pMetaType());
#else
            auto metaObj = metaType.pMetaType().metaObject();
#endif
            return serialize(*reinterpret_cast<const void *const *>(origin.constData()), metaObj, origin);
        } else {
            return origin;
        }
    }
    return origin;
}

QVariant JsonUtils::serialize(QObject *obj, const QVariant &origin) noexcept
{
    if (obj == nullptr) {
        return origin;
    }
    auto metaObj = obj->metaObject();
    if (!isSerialize(metaObj)) {
        return origin;
    }
    return McJsonUtils::toJson(obj);
}

QVariant JsonUtils::serialize(const void *obj, const QMetaObject *metaObj, const QVariant &origin) noexcept
{
    if (!isSerialize(metaObj)) {
        return origin;
    }
    return McJsonUtils::toJson(obj, metaObj);
}

#ifdef MC_USE_QT5
QVariant JsonUtils::deserialize(const QVariant &origin, int qmetaType) noexcept
#else
QVariant JsonUtils::deserialize(const QVariant &origin, const QMetaType &qmetaType) noexcept
#endif
{
    auto metaType = McMetaType::fromFuzzyQMetaType(qmetaType);
    if (metaType.isValid()) {
#ifdef MC_USE_QT5
        auto metaObj = QMetaType::metaObjectForType(metaType.pMetaType());
#else
        auto metaObj = metaType.pMetaType().metaObject();
#endif
        if (!isSerialize(metaObj)) {
            return origin;
        }
        return McJsonUtils::fromJson(qmetaType, origin);
    }
    return origin;
}

bool JsonUtils::isSerialize(const QMetaObject *metaObj) noexcept
{
    if (metaObj == nullptr) {
        return false;
    }
    auto serializeIndex = metaObj->indexOfClassInfo(MC_SERIALIZATION_TAG);
    if (serializeIndex == -1) {
        return false;
    }
    auto classInfo = metaObj->classInfo(serializeIndex);
    if (qstrcmp(classInfo.value(), MC_JSON_SERIALIZATION_TAG) != 0) {
        return false;
    }
    return true;
}

Q_GLOBAL_STATIC(JsonUtils, mcJsonUtils)
} // namespace

QJsonValue McJsonUtils::toJson(const QVariant &var) noexcept
{
#ifdef MC_USE_QT5
    auto qmetaType = var.userType();
#else
    auto qmetaType = var.metaType();
#endif
    auto metaType = McMetaType::fromFuzzyQMetaType(qmetaType);
    if (metaType.isValid()) {
#ifdef MC_USE_QT5
        auto flags = QMetaType::typeFlags(metaType.pMetaType());
#else
        auto flags = metaType.pMetaType().flags();
#endif
        if (flags.testFlag(QMetaType::PointerToQObject)) {
            if (metaType.pMetaType() == qmetaType) {
                return toJson(var.value<QObject *>());
            } else if (metaType.sMetaType() == qmetaType) {
                return toJson(var.value<QObjectPtr>());
#ifdef MC_USE_QT6
            } else if (metaType.wMetaType() == qmetaType) {
                return toJson(var.value<QWeakPointer<QObject>>().toStrongRef().data());
            } else if (metaType.tMetaType() == qmetaType) {
                return toJson(var.value<QPointer<QObject>>().data());
#endif
            }
        } else if (flags.testFlag(QMetaType::PointerToGadget)) {
#ifdef MC_USE_QT5
            auto metaObj = QMetaType::metaObjectForType(metaType.pMetaType());
#else
            auto metaObj = metaType.pMetaType().metaObject();
#endif
            return toJson(*reinterpret_cast<const void *const *>(var.constData()), metaObj);
        } else {
            return QJsonValue();
        }
    }
    auto listMetaType = McListMetaType::fromQMetaType(qmetaType);
    if (listMetaType.isValid()) {
        return toJson(var.value<QVariantList>());
    }
    auto mapMetaType = McMapMetaType::fromQMetaType(qmetaType);
#ifdef MC_USE_QT5
    if (mapMetaType.isValid() && mapMetaType.keyMetaType() == qMetaTypeId<QString>()) {
#else
    if (mapMetaType.isValid() && mapMetaType.keyMetaType() == QMetaType::fromType<QString>()) {
#endif
        return toJson(var.value<QVariantMap>());
    }
    return QJsonValue::fromVariant(var);
}

QJsonObject McJsonUtils::toJson(QObject *obj) noexcept
{
    if (obj == nullptr) {
        return QJsonObject();
    }
    QJsonObject jsonObj;
    const QMetaObject *mobj = obj->metaObject();
    for (int i = 0; i < mobj->propertyCount(); ++i) {
        QMetaProperty pro = mobj->property(i);
        QVariant variant = pro.read(obj);
        QJsonValue jsonValue = toJson(variant);
        jsonObj.insert(pro.name(), jsonValue);
    }
    return jsonObj;
}

QJsonArray McJsonUtils::toJson(const QVariantList &vars) noexcept
{
    QJsonArray jsonArr;

    for (auto &var : vars) {
        jsonArr.append(toJson(var));
    }

    return jsonArr;
}

QJsonObject McJsonUtils::toJson(const QVariantMap &objs) noexcept
{
    QJsonObject jsonObj;

    QMapIterator<QString, QVariant> itr(objs);
    while (itr.hasNext()) {
        auto item = itr.next();
        auto key = item.key();
        auto value = item.value();
        jsonObj.insert(key, toJson(value));
    }

    return jsonObj;
}

QJsonObject McJsonUtils::toJson(const void *gadget, const QMetaObject *mobj) noexcept
{
    if (gadget == nullptr || mobj == nullptr) {
        return QJsonObject();
    }
    QJsonObject jsonObj;
    for (int i = 0; i < mobj->propertyCount(); ++i) {
        QMetaProperty pro = mobj->property(i);
        QVariant variant = pro.readOnGadget(gadget);
        QJsonValue jsonValue = toJson(variant);
        jsonObj.insert(pro.name(), jsonValue);
    }
    return jsonObj;
}

QVariant McJsonUtils::fromJson(const QByteArray &typeName, const QVariant &value) noexcept
{
#ifdef MC_USE_QT5
    return McJsonUtils::fromJson(QMetaType::type(typeName), value);
#else
    return McJsonUtils::fromJson(QMetaType::fromName(typeName), value);
#endif
}

#ifdef MC_USE_QT5
QVariant McJsonUtils::fromJson(int qmetaType, const QVariant &value) noexcept
#else
QVariant McJsonUtils::fromJson(const QMetaType &qmetaType, const QVariant &value) noexcept
#endif
{
    return mcJsonUtils->makeValue(qmetaType, value);
}

QVariant McJsonUtils::serialize(const QVariant &origin) noexcept
{
    return mcJsonUtils->serialize(origin);
}

#ifdef MC_USE_QT5
QVariant McJsonUtils::deserialize(const QVariant &origin, int qmetaType) noexcept
#else
QVariant McJsonUtils::deserialize(const QVariant &origin, const QMetaType &qmetaType) noexcept
#endif
{
    return mcJsonUtils->deserialize(origin, qmetaType);
}
