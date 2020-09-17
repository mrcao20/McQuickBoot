#include "McBoot/Utils/McJsonUtils.h"

#include <QMetaProperty>
#include <QDebug>

#include "McIoc/BeanFactory/impl/McMetaTypeId.h"

QJsonValue McJsonUtils::toJson(const QVariant &var) noexcept
{
    auto type = QMetaType::type(var.typeName());
    auto str = QString("type '%1' is not registered!!!").arg(var.typeName());
    Q_ASSERT_X(type != QMetaType::UnknownType, "McJsonUtils::toJson", qPrintable(str));
    QJsonValue jsonValue;
    if (type >= QMetaType::User) {
        auto seqMetaTypeIds = McMetaTypeId::sequentialIds();
        auto assMetaTypeIds = McMetaTypeId::associativeIds();
        auto flags = QMetaType::typeFlags(type);
        if(flags.testFlag(QMetaType::TypeFlag::PointerToQObject)) {
            auto variant = McJsonUtils::toJson(var.value<QObject *>());
            jsonValue = QJsonValue::fromVariant(variant);
        }else if(flags.testFlag(QMetaType::TypeFlag::SharedPointerToQObject)){
            auto variant = McJsonUtils::toJson(var.value<QObjectPtr>());
            jsonValue = QJsonValue::fromVariant(variant);
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
    for(auto key : objs.keys()) {
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
#endif
