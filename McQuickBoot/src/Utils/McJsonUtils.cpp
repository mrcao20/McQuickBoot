#include "McBoot/Utils/McJsonUtils.h"

#include <QMetaProperty>
#include <QDebug>

#include <McIoc/BeanFactory/impl/McMetaTypeId.h>

namespace McPrivate {

struct JsonUtils
{
    QVariant makeValue(const QByteArray &typeName, const QVariant &arg) noexcept;

    QVariant makePlanValue(const QByteArray &typeName, const QVariant &arg) noexcept;
    QVariant makeListValue(const QVariant &arg, McSequentialMetaIdConstPtrRef seqMetaId) noexcept;
    QVariant makeMapValue(const QVariant &arg, McAssociativeMetaIdConstPtrRef assMetaId) noexcept;
    QVariant makeObjectValue(const QByteArray &typeName, const QVariant &arg) noexcept;
    QVariant makeGadgetValue(int type, const QVariant &arg) noexcept;
    int convertToObjectTypeId(const QByteArray &typeName) noexcept;

    QVariant serialize(const QVariant &origin) noexcept;
    QVariant serialize(QObject *obj, const QVariant &origin) noexcept;
    QVariant serialize(const void *obj, const QMetaObject *metaObj, const QVariant &origin) noexcept;
    QVariant deserialize(const QVariant &origin, int toId) noexcept;
    QVariant deserialize(const QMetaObject *metaObj, const QVariant &origin, int toId) noexcept;
    bool isSerialize(const QMetaObject *metaObj) noexcept;
};

QVariant JsonUtils::makeValue(const QByteArray &typeName, const QVariant &arg) noexcept
{
    auto type = QMetaType::type(typeName);
    auto flags = QMetaType::typeFlags(type);
    auto seqMetaTypeIds = McMetaTypeId::sequentialIds();
    auto assMetaTypeIds = McMetaTypeId::associativeIds();
    auto gadgetIds = McMetaTypeId::gadgetIds();
    for (auto gadgetId : gadgetIds) {
        if (gadgetId->pointerId == type) {
            return makeGadgetValue(gadgetId->gadgetId, arg);
        } else if (gadgetId->sharedId == type) {
            auto value = makeGadgetValue(gadgetId->gadgetId, arg);
            value.convert(type);
            return value;
        }
    }
    if (flags.testFlag(QMetaType::TypeFlag::SharedPointerToQObject)
        || flags.testFlag(QMetaType::TypeFlag::PointerToQObject)) {
        return makeObjectValue(typeName, arg);
    } else if (seqMetaTypeIds.contains(type)
               && seqMetaTypeIds.value(type)->valueId >= QMetaType::User) {
        return makeListValue(arg, seqMetaTypeIds.value(type));
    } else if (assMetaTypeIds.contains(type)
               && assMetaTypeIds.value(type)->valueId >= QMetaType::User) {
        return makeMapValue(arg, assMetaTypeIds.value(type));
    } else {
        return makePlanValue(typeName, arg);
    }
}

QVariant JsonUtils::makePlanValue(const QByteArray &typeName, const QVariant &arg) noexcept
{
    auto value = arg;
    if (typeName == MC_STRINGIFY(QJsonObject) && arg.type() == QVariant::Map) {
        value = QJsonObject::fromVariantMap(arg.value<QVariantMap>());
    } else if (typeName == MC_STRINGIFY(QJsonArray) && arg.type() == QVariant::List) {
        value = QJsonArray::fromVariantList(arg.value<QVariantList>());
    } else if (!value.convert(QMetaType::type(typeName))) {
        qCritical() << "property convert failure. origin type name:" << arg.typeName()
                    << "typeid:" << arg.type() << "target typeName:" << typeName;
        return arg;
    }
    return value;
}

QVariant JsonUtils::makeListValue(const QVariant &arg,
                                  McSequentialMetaIdConstPtrRef seqMetaId) noexcept
{
    QVariantList resList;
    auto list = arg.toList();
    auto valueId = seqMetaId->valueId;
    auto valueTypeName = QMetaType::typeName(valueId);
    for (auto var : list) {
        resList.append(makeValue(valueTypeName, var));
    }
    return resList;
}

QVariant JsonUtils::makeMapValue(const QVariant &arg,
                                 McAssociativeMetaIdConstPtrRef assMetaId) noexcept
{
    QMap<QVariant, QVariant> resMap;
    auto map = arg.toMap();
    auto keyId = assMetaId->keyId;
    auto valueId = assMetaId->valueId;
    auto keyTypeName = QMetaType::typeName(keyId);
    auto valueTypeName = QMetaType::typeName(valueId);
    QMapIterator<QString, QVariant> itr(map);
    while (itr.hasNext()) {
        auto item = itr.next();
        resMap.insert(makeValue(keyTypeName, item.key()), makeValue(valueTypeName, item.value()));
    }
    return QVariant::fromValue(resMap);
}

QVariant JsonUtils::makeObjectValue(const QByteArray &typeName, const QVariant &arg) noexcept
{
    auto typeId = convertToObjectTypeId(typeName);
    QByteArray objTypeName = QMetaType::typeName(typeId);

    if (typeId == QMetaType::UnknownType) {
        qCritical("this class for type '%s' is not register\n", objTypeName.data());
        return QVariant();
    }
    const QMetaObject *mobj = QMetaType::metaObjectForType(typeId);
    QObject *obj = mobj->newInstance();
    int count = mobj->propertyCount();
    auto args = arg.toMap();
    for (int i = 0; i < count; ++i) {
        QMetaProperty pro = mobj->property(i);
        auto name = pro.name();
        if (!args.contains(name))
            continue;
        auto value = args[name];
        value = makeValue(pro.typeName(), value);
        if (!pro.write(obj, value))
            qCritical("cannot dynamic write value to property '%s' for class '%s'\n",
                      pro.name(),
                      objTypeName.data());
    }
    auto qobjectPointerIds = McMetaTypeId::qobjectPointerIds();
    auto type = QMetaType::type(typeName);
    if (qobjectPointerIds.contains(type)) {
        QVariant var;
        var.setValue(obj);
        var.convert(typeId);
        return var;
    } else {
        QObjectPtr objPtr(obj);
        QVariant var;
        var.setValue(objPtr);
        var.convert(qobjectPointerIds.value(typeId)->sharedPointerId);
        return var;
    }
}

QVariant JsonUtils::makeGadgetValue(int type, const QVariant &arg) noexcept
{
    auto gadget = QMetaType::create(type);
    auto mobj = QMetaType::metaObjectForType(type);
    int count = mobj->propertyCount();
    auto args = arg.toMap();
    for (int i = 0; i < count; ++i) {
        QMetaProperty pro = mobj->property(i);
        auto name = pro.name();
        if (!args.contains(name))
            continue;
        auto value = args[name];
        value = makeValue(pro.typeName(), value);
        if (!pro.writeOnGadget(gadget, value))
            qCritical("cannot dynamic write value to property '%s' for class '%s'\n",
                      pro.name(),
                      mobj->className());
    }
    auto gadgetIds = McMetaTypeId::gadgetIds();
    QVariant value(gadgetIds.value(type)->pointerId, &gadget);
    return value;
}

int JsonUtils::convertToObjectTypeId(const QByteArray &typeName) noexcept
{
    auto qobjectPointerIds = McMetaTypeId::qobjectPointerIds();
    auto sharedPointerIds = McMetaTypeId::sharedPointerIds();
    auto type = QMetaType::type(typeName);
    if (qobjectPointerIds.contains(type)) {
        return type;
    } else if (sharedPointerIds.contains(type)) {
        return sharedPointerIds.value(type)->qobjectPointerId;
    } else {
        return QMetaType::UnknownType;
    }
}

QVariant JsonUtils::serialize(const QVariant &origin) noexcept
{
    auto originId = origin.userType();
    auto qobjectIds = McMetaTypeId::qobjectPointerIds();
    auto sharedIds = McMetaTypeId::sharedPointerIds();
    auto gadgetIds = McMetaTypeId::gadgetIds();
    for (auto gadgetId : gadgetIds) {
        if (gadgetId->pointerId == originId || gadgetId->sharedId == originId) {
            return serialize(*reinterpret_cast<const void *const *>(origin.constData()),
                             QMetaType::metaObjectForType(gadgetId->gadgetId),
                             origin);
        }
    }
    if (qobjectIds.contains(originId)) {
        return serialize(origin.value<QObject *>(), origin);
    } else if (sharedIds.contains(originId)) {
        return serialize(origin.value<QObjectPtr>().data(), origin);
    } else {
        return origin;
    }
}

QVariant JsonUtils::serialize(QObject *obj, const QVariant &origin) noexcept
{
    auto metaObj = obj->metaObject();
    if (!isSerialize(metaObj)) {
        return origin;
    }
    return McJsonUtils::toJson(obj);
}

QVariant JsonUtils::serialize(const void *obj,
                              const QMetaObject *metaObj,
                              const QVariant &origin) noexcept
{
    if (!isSerialize(metaObj)) {
        return origin;
    }
    return McJsonUtils::toJson(obj, metaObj);
}

QVariant JsonUtils::deserialize(const QVariant &origin, int toId) noexcept
{
    auto qobjectIds = McMetaTypeId::qobjectPointerIds();
    auto sharedIds = McMetaTypeId::sharedPointerIds();
    auto gadgetIds = McMetaTypeId::gadgetIds();
    for (auto gadgetId : gadgetIds) {
        if (gadgetId->pointerId == toId || gadgetId->sharedId == toId) {
            auto metaObj = QMetaType::metaObjectForType(gadgetId->gadgetId);
            return deserialize(metaObj, origin, toId);
        }
    }
    if (qobjectIds.contains(toId)) {
        auto metaObj = QMetaType::metaObjectForType(toId);
        return deserialize(metaObj, origin, toId);
    } else if (sharedIds.contains(toId)) {
        auto metaObj = QMetaType::metaObjectForType(sharedIds.value(toId)->qobjectPointerId);
        return deserialize(metaObj, origin, toId);
    } else {
        return origin;
    }
}

QVariant JsonUtils::deserialize(const QMetaObject *metaObj,
                                const QVariant &origin,
                                int toId) noexcept
{
    if (!isSerialize(metaObj)) {
        return origin;
    }
    return McJsonUtils::fromJson(toId, origin);
}

bool JsonUtils::isSerialize(const QMetaObject *metaObj) noexcept
{
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

} // namespace McPrivate

MC_GLOBAL_STATIC(McPrivate::JsonUtils, mcJsonUtils)

QJsonValue McJsonUtils::toJson(const QVariant &var) noexcept
{
    auto type = var.userType();
    QJsonValue jsonValue;
    if (type >= QMetaType::User) {
        auto seqMetaTypeIds = McMetaTypeId::sequentialIds();
        auto assMetaTypeIds = McMetaTypeId::associativeIds();
        auto gadgetIds = McMetaTypeId::gadgetIds();
        for (auto gadgetId : gadgetIds) {
            if (gadgetId->pointerId == type || gadgetId->sharedId == type) {
                return toJson(*reinterpret_cast<const void *const *>(var.constData()),
                              QMetaType::metaObjectForType(gadgetId->gadgetId));
            }
        }
        auto flags = QMetaType::typeFlags(type);
        if(flags.testFlag(QMetaType::TypeFlag::PointerToQObject)) {
            jsonValue = McJsonUtils::toJson(var.value<QObject *>());
        }else if(flags.testFlag(QMetaType::TypeFlag::SharedPointerToQObject)){
            jsonValue = McJsonUtils::toJson(var.value<QObjectPtr>());
        }else if(seqMetaTypeIds.contains(type)) {
            QVariantList varList = var.value<QVariantList>();
            jsonValue = toJson(varList);
        }else if(assMetaTypeIds.contains(type)) {
            QVariantMap varMap = var.value<QVariantMap>();
            jsonValue = toJson(varMap);
        }else{
            qInfo("not support type: %d, typeName: %s\n", type, var.typeName());
        }
    }else{
        jsonValue = QJsonValue::fromVariant(var);
    }
    return jsonValue;
}

QJsonObject McJsonUtils::toJson(QObject *obj) noexcept 
{
    if (!obj)
        return QJsonObject();
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

QJsonObject McJsonUtils::toJson(QObjectConstPtrRef obj) noexcept 
{
    return McJsonUtils::toJson(obj.data());
}

QJsonArray McJsonUtils::toJson(const QVariantList &vars) noexcept
{
    QJsonArray jsonArr;
    
    for(auto var : vars) {
        jsonArr.append(toJson(var));
    }
    
    return jsonArr;
}

QJsonObject McJsonUtils::toJson(const QMap<QString, QObject *> &objs) noexcept 
{
    QJsonObject result;
    for(auto key : objs.keys()) {
        result.insert(key, McJsonUtils::toJson(objs.value(key)));
    }
    return result;
}

QJsonObject McJsonUtils::toJson(const QMap<QString, QObjectPtr> &objs) noexcept 
{
    QJsonObject result;
    const auto &keys = objs.keys();
    for (const auto &key : keys) {
        result.insert(key, McJsonUtils::toJson(objs.value(key)));
    }
    return result;
}

QJsonObject McJsonUtils::toJson(const QVariantMap &objs) noexcept
{
    QJsonObject jsonObj;
    
    QMapIterator<QString, QVariant> itr(objs);
    while(itr.hasNext()) {
        auto item = itr.next();
        auto key = item.key();
        auto value = item.value();
        jsonObj.insert(key, toJson(value));
    }
    
    return jsonObj;
}

#if QT_VERSION >= QT_VERSION_CHECK(5, 6, 0)
QJsonObject McJsonUtils::toJson(const void *gadget, const QMetaObject *mobj) noexcept
{
    QJsonObject jsonObj;
    for (int i = 0; i < mobj->propertyCount(); ++i) {
        QMetaProperty pro = mobj->property(i);
        QVariant variant = pro.readOnGadget(gadget);
        QJsonValue jsonValue = toJson(variant);
        jsonObj.insert(pro.name(), jsonValue);
    }
    return jsonObj;
}
#endif

QVariant McJsonUtils::fromJson(const QByteArray &typeName, const QVariant &value) noexcept
{
    return mcJsonUtils->makeValue(typeName, value);
}

QVariant McJsonUtils::fromJson(int type, const QVariant &value) noexcept
{
    return McJsonUtils::fromJson(QMetaType::typeName(type), value);
}

QVariant McJsonUtils::serialize(const QVariant &origin) noexcept
{
    return mcJsonUtils->serialize(origin);
}

QVariant McJsonUtils::deserialize(const QVariant &origin, int toId) noexcept
{
    return mcJsonUtils->deserialize(origin, toId);
}
