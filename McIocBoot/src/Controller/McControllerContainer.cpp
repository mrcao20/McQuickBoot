#include "McBoot/Controller/impl/McControllerContainer.h"

#include <QMetaMethod>
#include <QJsonObject>
#include <QQmlEngine>
#include <QDebug>

#include <McIoc/ApplicationContext/IMcApplicationContext.h>

#include "McBoot/McIocBoot.h"
#include "McBoot/Controller/impl/McResult.h"
#include "McIoc/BeanFactory/impl/McMetaTypeId.h"

MC_DECL_PRIVATE_DATA(McControllerContainer)
QMap<QString, QObjectPtr> controllers;	//!< 键为beanName，值为controller对象
MC_DECL_PRIVATE_DATA_END

McControllerContainer::McControllerContainer(QObject *parent)
	: QObject(parent)
{
    MC_NEW_PRIVATE_DATA(McControllerContainer)
}

McControllerContainer::~McControllerContainer()
{
}

void McControllerContainer::init(const QSharedPointer<McIocBoot> &boot) noexcept 
{
    auto appCtx = boot->getApplicationContext();
	auto beanNames = boot->getComponents(MC_CONTROLLER);
    for (const auto &beanName : beanNames) {
        auto obj = appCtx->getBean(beanName);
        if(!obj) {
            qCritical() << QString("controller for named '%1' not exists").arg(beanName);
            continue;
        }
        d->controllers.insert(beanName, obj);
    }
}

QVariant McControllerContainer::invoke(const QString &uri, const QVariant &body) noexcept 
{
    switch (static_cast<QMetaType::Type>(body.type())) {
    case QMetaType::Type::UnknownType:
        return invoke(uri);
    case QMetaType::Type::QJsonObject:
        return invoke(uri, body.toJsonObject());
    default:
        return fail("this body is not support type");
    }
}

QVariant McControllerContainer::invoke(const QString &uri) noexcept 
{
    auto list = uri.split('?', Qt::SkipEmptyParts);
    if (list.isEmpty())
        return fail("access path not exists");
    QObjectPtr bean;
    QString func;
    QVariant errRet;
    if(!splitBeanAndFunc(list.at(0), bean, func, errRet)) {
        return errRet;
    }
    QString param = list.size() == 2 ? list.at(1) : "";
    // <参数名，参数值>
    QMap<QString, QVariant> args = splitParam(param);
    return invokeForUri(bean, func, args);
}

QVariant McControllerContainer::invoke(const QString &uri, const QJsonObject &data) noexcept 
{
    QObjectPtr bean;
    QString func;
    QVariant errRet;
    if(!splitBeanAndFunc(uri, bean, func, errRet)) {
        return errRet;
    }
    // <参数名，参数值>
    QMap<QString, QVariant> args = data.toVariantMap();
    return invokeForUri(bean, func, args);
}

bool McControllerContainer::splitBeanAndFunc(
        const QString &uri
        , QObjectPtr &bean
        , QString &func
        , QVariant &errRet) noexcept 
{
    auto beanAndFunc = uri.split('.', Qt::SkipEmptyParts);
    if (beanAndFunc.size() != 2) {
        errRet = fail("access path not exists");
        return false;
    }
    if(!d->controllers.contains(beanAndFunc.at(0))) {
        errRet = fail("access path not exists");
        return false;
    }
    bean = d->controllers[beanAndFunc.at(0)];
    func = beanAndFunc.at(1);
    return true;
}

QVariant McControllerContainer::invokeForUri(
        QObjectConstPtrRef bean
        , const QString &func
        ,  const QMap<QString, QVariant> &args) noexcept 
{
    const QMetaObject *metaBean = bean->metaObject();
    int count = metaBean->methodCount();
    for (int i = 0; i < count; ++i) {
        QMetaMethod method = metaBean->method(i);
        if(method.name() != func)
            continue;
        if (!isMethodMatching(method, args.keys()))
            continue;
        return invokeForArgs(bean, method, args);
    }
    return fail("access arguments error");
}

QMap<QString, QVariant> McControllerContainer::splitParam(const QString &param) noexcept 
{
    QMap<QString, QVariant> args;
    QStringList params = param.split('&', Qt::SkipEmptyParts);
    for (const auto &p : params) {
        QStringList nameAndValue = p.split('=', Qt::SkipEmptyParts);
        if(nameAndValue.isEmpty())
            continue;
        QString key = nameAndValue.at(0);
        QVariant value;
        if(nameAndValue.size() == 2)
            value = nameAndValue.at(1);
        else
            value = "";
        args.insert(key, value);
    }
    return args;
}

bool McControllerContainer::isMethodMatching(
        const QMetaMethod &m
        , const QList<QString> &names) noexcept 
{
    QList<QByteArray> methodParamNames = m.parameterNames();
    if (methodParamNames.size() > names.size())
        return false;
    QList<QString> argNames = names;
    if (methodParamNames.size() == argNames.size()) {
        QList<QByteArray> list;
        removeDuplication(argNames, methodParamNames, list);
        if (methodParamNames.isEmpty())
            return true;
        else
            return false;
    }
    else {
        QList<QByteArray> methodParamTypes = m.parameterTypes();
        removeDuplication(argNames, methodParamNames, methodParamTypes);
        if (methodParamNames.isEmpty())
            return false;
        for (QByteArray type : methodParamTypes) {
            if (!type.endsWith("Ptr") 
                    && !type.endsWith("ConstPtrRef"))
                return false;
            if(type.endsWith("Ptr")) {
                type.remove(type.length() - 3, 3);
            }else{
                type.remove(type.length() - 11, 11);
            }
            int typeId = QMetaType::type(type);
            if (typeId == QMetaType::UnknownType)
                return false;
            const QMetaObject *mobj = QMetaType::metaObjectForType(typeId);
            for (const QString &name : argNames) {
                if (mobj->indexOfProperty(name.toLocal8Bit()) == -1)
                    continue;
                argNames.removeOne(name);
            }
        }
        if (argNames.isEmpty())
            return true;
        else
            return false;
    }
}

void McControllerContainer::removeDuplication(
        QList<QString> &argNames
        , QList<QByteArray> &paramNames, QList<QByteArray> &paramTypes) noexcept 
{
    for (int i = 0; i < paramNames.size(); ++i) {
        if (!argNames.contains(paramNames.at(i)))
            continue;
        argNames.removeOne(paramNames.at(i));
        paramNames.removeAt(i);
        paramTypes.removeAt(i);
        --i;
    }
}

QVariant McControllerContainer::invokeForArgs(
        QObjectConstPtrRef bean
        , const QMetaMethod &method
        , const QMap<QString, QVariant> &args) noexcept 
{
    auto returnType = method.returnType();
    if(strcmp(method.typeName(), "QAbstractItemModel*") == 0
            || strcmp(method.typeName(), "McResult*") == 0) {
        
        returnType = QMetaType::type("QObject*");
    }
    if(returnType == QMetaType::Type::UnknownType) {
        qCritical() << "if you want to return a model to QML. the return type "
                       "must be QObject* or QAbstractItemModel*";
        return fail("cannot found this return type from meta object system");
    }
    QVariant returnValue;
    QGenericReturnArgument returnArg;
    if (method.returnType() == QMetaType::Void)
        returnValue = "调用成功";
    else {
        returnValue = QVariant(static_cast<QVariant::Type>(returnType));
        // 由于调用函数时需要对比类型名，所以这里类型名需要具体指定，不能再用QVariant
//		returnArg = Q_RETURN_ARG(QVariant, returnValue);
        returnArg = QGenericReturnArgument(method.typeName(), returnValue.data());
    }
    QVector<QGenericArgument> arguments;
    arguments.resize(10);
	
    bool ok = false;
    QVariant errMsg;
    auto values = makeValues(method, args, arguments.size(), &errMsg, &ok);
    if(!ok) {
        return errMsg;
    }
    for(int i = 0; i < values.size(); ++i) {
        // 由于Q_ARG宏所调用的函数是直接取value的地址，所以value不能是局部变量
//		arguments.replace(i, Q_ARG(QVariant, values[i]));
        /* 由于业务需要，调用函数的方式为直接连接，通过源代码分析，直接连接的类型名
         * 不需要使用，只需要保证它不为空即可
         */
        arguments.replace(i, QGenericArgument("QVariant", values[i].data()));
    }
    if (!method.invoke(bean.data(),
                       Qt::DirectConnection, 
                       returnArg,
                       // 由于Q_ARG宏所调用的函数是直接取value的地址，所以value不能是局部变量
                       arguments.at(0),
                       arguments.at(1),
                       arguments.at(2),
                       arguments.at(3),
                       arguments.at(4),
                       arguments.at(5),
                       arguments.at(6),
                       arguments.at(7),
                       arguments.at(8),
                       arguments.at(9)))
        return fail("failed invoke function");
    
    if(strcmp(method.typeName(), "McResult*") == 0) {
        McResult *result = returnValue.value<McResult *>();
        QQmlEngine::setObjectOwnership(result, QQmlEngine::JavaScriptOwnership);
    }
    return returnValue;
}

QVariantList McControllerContainer::makeValues(const QMetaMethod &method
                        , const QMap<QString, QVariant> &args
                        , int maxParamSize
                        , QVariant *errMsg
                        , bool *ok) noexcept 
{
    bool isOk = false;
    QVariant msg;
    if(ok == nullptr) {
        ok = &isOk;
    }
    if(errMsg == nullptr) {
        errMsg = &msg;
    }
    QList<QByteArray> paramNames = method.parameterNames(); //!< 和类型名数量一定相等
    QList<QByteArray> paramTypes = method.parameterTypes();
    if(paramTypes.size() > maxParamSize) {
        *errMsg = fail(QString("argument for function cannot more than %1").arg(maxParamSize));
        *ok = false;
        return QVariantList();
    }
    QVariantList list;
    for (int i = 0; i < paramTypes.size(); ++i) {
        QByteArray type = paramTypes.at(i).simplified();
        QByteArray name = paramNames.at(i).simplified();
        QVariant value = args[name];     //!< 由于调用此函数时参数名一定存在，所以这里一定有值
        list.append(makeValue(type, value));
    }
    *ok = true;
    return list;
}

QVariant McControllerContainer::makeValue(
        const QByteArray &typeName
        , const QVariant &arg) noexcept
{
    auto type = QMetaType::type(typeName);
    auto flags = QMetaType::typeFlags(type);
    auto seqMetaTypeIds = McMetaTypeId::sequentialIds();
    auto assMetaTypeIds = McMetaTypeId::associativeIds();
    if(flags.testFlag(QMetaType::TypeFlag::SharedPointerToQObject)
            || flags.testFlag(QMetaType::TypeFlag::PointerToQObject)) {
        return makeObjectValue(typeName, arg);
    } else if(seqMetaTypeIds.contains(type)) {
        return makeListValue(arg, seqMetaTypeIds.value(type));
    } else if(assMetaTypeIds.contains(type)) {
        return makeMapValue(arg, assMetaTypeIds.value(type));
    } else {
        return makePlanValue(typeName, arg);
    }
}

QVariant McControllerContainer::makePlanValue(
        const QByteArray &typeName
        , const QVariant &arg) noexcept
{
    auto value = arg;
    if(!value.convert(QMetaType::type(typeName))) {
        qCritical() << "property convert failure. origin type name:" << arg.typeName()
                    << "typeid:" << arg.type();
        return arg;
    }
    return value;
}

QVariant McControllerContainer::makeListValue(
        const QVariant &arg
        , McSequentialMetaIdConstPtrRef seqMetaId) noexcept
{
    QVariantList resList;
    auto list = arg.toList();
    auto valueId = seqMetaId->valueId;
    auto valueTypeName = QMetaType::typeName(valueId);
    for(auto var : list) {
        resList.append(makeValue(valueTypeName, var));
    }
    return resList;
}

QVariant McControllerContainer::makeMapValue(
        const QVariant &arg
        , McAssociativeMetaIdConstPtrRef assMetaId) noexcept
{
    QMap<QVariant, QVariant> resMap;
    auto map = arg.toMap();
    auto keyId = assMetaId->keyId;
    auto valueId = assMetaId->valueId;
    auto keyTypeName = QMetaType::typeName(keyId);
    auto valueTypeName = QMetaType::typeName(valueId);
    QMapIterator<QString, QVariant> itr(map);
    while(itr.hasNext()) {
        auto item = itr.next();
        resMap.insert(makeValue(keyTypeName, item.key()), makeValue(valueTypeName, item.value()));
    }
    return QVariant::fromValue(resMap);
}

QVariant McControllerContainer::makeObjectValue(
        const QByteArray &typeName
        , const QVariant &arg) noexcept 
{
    QByteArray objTypeName;
    if(!isSharedPointerObject(typeName, objTypeName)) {
        return arg;
    }
    
    int typeId = QMetaType::type(objTypeName);
    if (typeId == QMetaType::UnknownType) {
        qCritical("this class for type '%s' is not register", objTypeName.data());
        return QVariant();
    }
    const QMetaObject *mobj = QMetaType::metaObjectForType(typeId);
    QObject *obj = mobj->newInstance();
    int count = mobj->propertyCount();
    auto args = arg.toMap();
    for (int i = 0; i < count; ++i) {
        QMetaProperty pro = mobj->property(i);
        auto name = pro.name();
        if(!args.contains(name))
            continue;
        auto value = args[name];
        value = makeValue(pro.typeName(), value);
        if (!pro.write(obj, value))
            qCritical("cannot dynamic write value to property '%s' for class '%s'", objTypeName.data(), pro.name());
    }
    QObjectPtr objPtr(obj);
    QVariant var;
    var.setValue(objPtr);
    var.convert(QMetaType::type(objTypeName.append("Ptr")));
    return var;
}

bool McControllerContainer::isSharedPointerObject(
        const QByteArray &typeName
        , QByteArray &objTypeName) noexcept 
{
    objTypeName = typeName;
    auto qobjectPointerIds = McMetaTypeId::qobjectPointerIds();
    auto sharedPointerIds = McMetaTypeId::sharedPointerIds();
    auto type = QMetaType::type(typeName);
    if(qobjectPointerIds.contains(type)) {
        objTypeName.remove(objTypeName.length() - 1, 1);
        return true;
    } else if(sharedPointerIds.contains(type)) {
        if(typeName.endsWith("Ptr")) {
            objTypeName.remove(objTypeName.length() - 3, 3);
        } else {
            objTypeName.remove(objTypeName.length() - 11, 11);
        }
        return true;
    }
    return false;
}

QVariant McControllerContainer::ok(const QVariant &val) const noexcept 
{
    McResult *result = McResult::ok(val);
    
    QQmlEngine::setObjectOwnership(result, QQmlEngine::JavaScriptOwnership);
    
    return QVariant::fromValue(qobject_cast<QObject *>(result));
}

QVariant McControllerContainer::fail(const QString &val) const noexcept 
{
    McResult *result = McResult::fail(val);
    
    QQmlEngine::setObjectOwnership(result, QQmlEngine::JavaScriptOwnership);
    
    return QVariant::fromValue(qobject_cast<QObject *>(result));
}
