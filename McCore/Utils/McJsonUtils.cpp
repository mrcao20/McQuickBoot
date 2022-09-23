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
#include "McJsonUtils.h"

#ifdef MC_USE_QT6
# include <QMetaClassInfo>

namespace {
struct JsonUtils
{
    QVariant makeValue(const QMetaType &qmetaType, const QVariant &arg) noexcept;
    QVariant makeGadgetValue(const QMetaType &qmetaType, McMetaType metaType, const QVariant &arg) noexcept;
    QVariant makeObjectValue(const QMetaType &qmetaType, McMetaType metaType, const QVariant &arg) noexcept;
    QVariant makeListValue(McListMetaType metaType, const QVariant &arg) noexcept;
    QVariant makeMapValue(McMapMetaType metaType, const QVariant &arg) noexcept;
    QVariant makePlanValue(const QMetaType &qmetaType, const QVariant &arg) noexcept;

    QVariant serialize(const QVariant &origin) noexcept;
    QVariant serialize(QObject *obj, const QVariant &origin) noexcept;
    QVariant serialize(const void *obj, const QMetaObject *metaObj, const QVariant &origin) noexcept;
    QVariant deserialize(const QVariant &origin, const QMetaType &qmetaType) noexcept;

    bool isSerialize(const QMetaObject *metaObj) noexcept;
};

QVariant JsonUtils::makeValue(const QMetaType &qmetaType, const QVariant &arg) noexcept
{
    auto metaType = McMetaType::fromFuzzyQMetaType(qmetaType);
    if (metaType.isValid()) {
        if (metaType.pMetaType().flags().testFlag(QMetaType::PointerToQObject)) {
            return makeObjectValue(qmetaType, metaType, arg);
        } else if (metaType.pMetaType().flags().testFlag(QMetaType::PointerToGadget)) {
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
    if (mapMetaType.isValid() && mapMetaType.keyMetaType() == QMetaType::fromType<QString>()) {
        return makeMapValue(mapMetaType, arg);
    }
    return makePlanValue(qmetaType, arg);
}

QVariant JsonUtils::makeGadgetValue(const QMetaType &qmetaType, McMetaType metaType, const QVariant &arg) noexcept
{
    auto metaObj = metaType.pMetaType().metaObject();
    if (metaObj == nullptr) {
        return QVariant();
    }
    auto gadget = metaType.metaType().create();
    auto count = metaObj->propertyCount();
    auto args = arg.toMap();
    for (int i = 0; i < count; ++i) {
        QMetaProperty pro = metaObj->property(i);
        auto name = pro.name();
        if (!args.contains(name)) {
            continue;
        }
        auto value = args[name];
        value = makeValue(pro.metaType(), value);
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

QVariant JsonUtils::makeObjectValue(const QMetaType &qmetaType, McMetaType metaType, const QVariant &arg) noexcept
{
    auto metaObj = metaType.pMetaType().metaObject();
    if (metaObj == nullptr) {
        return QVariant();
    }
    QObject *obj = nullptr;
    QVariant ret;
    if (metaType.pMetaType() == qmetaType) {
        auto bean = metaType.metaType().create();
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
        value = makeValue(pro.metaType(), value);
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

QVariant JsonUtils::makePlanValue(const QMetaType &qmetaType, const QVariant &arg) noexcept
{
    if (qmetaType == QMetaType::fromType<QJsonObject>() && arg.metaType() == QMetaType::fromType<QVariantMap>()) {
        return QJsonObject::fromVariantMap(arg.value<QVariantMap>());
    } else if (qmetaType == QMetaType::fromType<QJsonArray>() && arg.metaType() == QMetaType::fromType<QVariantList>()) {
        return QJsonArray::fromVariantList(arg.value<QVariantList>());
    }
    auto value = arg;
    if (!value.convert(qmetaType)) {
        qCCritical(mcCore()) << "property convert failure. origin type name:" << arg.metaType().name()
                             << "target typeName:" << qmetaType.name();
        return arg;
    }
    return value;
}

QVariant JsonUtils::serialize(const QVariant &origin) noexcept
{
    auto qmetaType = origin.metaType();
    auto metaType = McMetaType::fromFuzzyQMetaType(qmetaType);
    if (metaType.isValid()) {
        if (metaType.pMetaType().flags().testFlag(QMetaType::PointerToQObject)) {
            if (metaType.pMetaType() == qmetaType) {
                return serialize(origin.value<QObject *>(), origin);
            } else if (metaType.sMetaType() == qmetaType) {
                return serialize(origin.value<QObjectPtr>().data(), origin);
            } else if (metaType.wMetaType() == qmetaType) {
                return serialize(origin.value<QWeakPointer<QObject>>().toStrongRef().data(), origin);
            } else if (metaType.tMetaType() == qmetaType) {
                return serialize(origin.value<QPointer<QObject>>().data(), origin);
            }
        } else if (metaType.pMetaType().flags().testFlag(QMetaType::PointerToGadget)) {
            return serialize(
                *reinterpret_cast<const void *const *>(origin.constData()), metaType.pMetaType().metaObject(), origin);
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

QVariant JsonUtils::deserialize(const QVariant &origin, const QMetaType &qmetaType) noexcept
{
    auto metaType = McMetaType::fromFuzzyQMetaType(qmetaType);
    if (metaType.isValid()) {
        if (!isSerialize(metaType.pMetaType().metaObject())) {
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
    auto qmetaType = var.metaType();
    auto metaType = McMetaType::fromFuzzyQMetaType(qmetaType);
    if (metaType.isValid()) {
        if (metaType.pMetaType().flags().testFlag(QMetaType::PointerToQObject)) {
            if (metaType.pMetaType() == qmetaType) {
                return toJson(var.value<QObject *>());
            } else if (metaType.sMetaType() == qmetaType) {
                return toJson(var.value<QObjectPtr>());
            } else if (metaType.wMetaType() == qmetaType) {
                return toJson(var.value<QWeakPointer<QObject>>().toStrongRef().data());
            } else if (metaType.tMetaType() == qmetaType) {
                return toJson(var.value<QPointer<QObject>>().data());
            }
        } else if (metaType.pMetaType().flags().testFlag(QMetaType::PointerToGadget)) {
            return toJson(*reinterpret_cast<const void *const *>(var.constData()), metaType.pMetaType().metaObject());
        } else {
            return QJsonValue();
        }
    }
    auto listMetaType = McListMetaType::fromQMetaType(qmetaType);
    if (listMetaType.isValid()) {
        return toJson(var.value<QVariantList>());
    }
    auto mapMetaType = McMapMetaType::fromQMetaType(qmetaType);
    if (mapMetaType.isValid() && mapMetaType.keyMetaType() == QMetaType::fromType<QString>()) {
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
    return McJsonUtils::fromJson(QMetaType::fromName(typeName), value);
}

QVariant McJsonUtils::fromJson(const QMetaType &qmetaType, const QVariant &value) noexcept
{
    return mcJsonUtils->makeValue(qmetaType, value);
}

QVariant McJsonUtils::serialize(const QVariant &origin) noexcept
{
    return mcJsonUtils->serialize(origin);
}

QVariant McJsonUtils::deserialize(const QVariant &origin, const QMetaType &qmetaType) noexcept
{
    return mcJsonUtils->deserialize(origin, qmetaType);
}
#endif
