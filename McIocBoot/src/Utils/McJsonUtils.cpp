#include "McBoot/Utils/McJsonUtils.h"

#include <QMetaProperty>
#include <QDebug>

#include "McIoc/BeanFactory/impl/McMetaTypeId.h"

QJsonObject McJsonUtils::toJson(QObject *obj) noexcept 
{
    if (!obj)
        return QJsonObject();
    QJsonObject jsonObj;
    const QMetaObject *mobj = obj->metaObject();
    auto seqMetaTypeIds = McMetaTypeId::sequentialIds();
    auto assMetaTypeIds = McMetaTypeId::associativeIds();
    for (int i = 0; i < mobj->propertyCount(); ++i) {
        QMetaProperty pro = mobj->property(i);
        auto type = static_cast<QMetaType::Type>(pro.type());
        if (type == QMetaType::UnknownType) {
            qInfo("type '%s' is not registered!!!\n", pro.typeName());
            continue;
        }
        QVariant variant = pro.read(obj);
        if (type >= QMetaType::User) {
            auto flags = QMetaType::typeFlags(static_cast<int>(pro.type()));
            if(flags.testFlag(QMetaType::TypeFlag::PointerToQObject)) {
                variant = McJsonUtils::toJson(variant.value<QObject *>());
            }else if(flags.testFlag(QMetaType::TypeFlag::SharedPointerToQObject)){
                variant = McJsonUtils::toJson(variant.value<QObjectPtr>());
            }else if(seqMetaTypeIds.contains(type)) {
                
            }else if(assMetaTypeIds.contains(type)) {
                
            }else{
                qInfo("not support type: %d, typeName: %s\n", type, pro.typeName());
                continue;
            }
        }
        QJsonValue jsonValue = QJsonValue::fromVariant(variant);
        jsonObj.insert(pro.name(), jsonValue);
    }
    return jsonObj;
}

QJsonObject McJsonUtils::toJson(QObjectConstPtrRef obj) noexcept 
{
    return McJsonUtils::toJson(obj.data());
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
    for(auto key : objs.keys()) {
        result.insert(key, McJsonUtils::toJson(objs.value(key)));
    }
    return result;
}

QJsonObject McJsonUtils::toJson(void *gadget, const QMetaObject *mobj) noexcept 
{
    QJsonObject jsonObj;
    for (int i = 0; i < mobj->propertyCount(); ++i) {
        QMetaProperty pro = mobj->property(i);
        if (static_cast<QMetaType::Type>(pro.type()) == QMetaType::UnknownType)
            continue;
        QVariant variant = pro.readOnGadget(gadget);
        QJsonValue jsonValue = QJsonValue::fromVariant(variant);
        jsonObj.insert(pro.name(), jsonValue);
    }
    return jsonObj;
}
