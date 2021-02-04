#include "McBoot/Controller/impl/McControllerContainer.h"

#include <QAbstractItemModel>
#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
#include <QMetaMethod>
#include <QQmlEngine>

#include <McIoc/ApplicationContext/IMcApplicationContext.h>
#include <McIoc/BeanFactory/impl/McMetaTypeId.h>

#include "McBoot/Controller/impl/McResult.h"
#include "McBoot/IMcQuickBoot.h"
#include "McBoot/Utils/Callback/Impl/McQmlSyncCallback.h"
#include "McBoot/Utils/McJsonUtils.h"

MC_INIT(McControllerContainer)
MC_REGISTER_BEAN_FACTORY(McControllerContainer)
qRegisterMetaType<QAbstractItemModel *>();
MC_INIT_END

MC_DECL_PRIVATE_DATA(McControllerContainer)
QMap<QString, QObjectPtr> controllers;    //!< 键为beanName，值为controller对象
MC_DECL_PRIVATE_DATA_END

McControllerContainer::McControllerContainer(QObject *parent)
    : QObject(parent)
{
    MC_NEW_PRIVATE_DATA(McControllerContainer)
}

McControllerContainer::~McControllerContainer()
{
}

void McControllerContainer::init(const IMcQuickBoot *boot) noexcept
{
    d->controllers.clear();
    auto appCtx = boot->getApplicationContext();
    auto beanNames = Mc::getComponents(appCtx, MC_CONTROLLER_TAG);
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
    case QMetaType::Type::QVariantList:
        return invoke(uri, body.toList());
    case QMetaType::Type::QVariantMap:
        return invoke(uri, body.toMap());
    default:
        return fail("this body is not support type");
    }
}

QVariant McControllerContainer::invoke(const QString &uri) noexcept
{
    return invoke(uri, QVariantMap());
}

QVariant McControllerContainer::invoke(const QString &uri, const QJsonObject &data) noexcept 
{
    // <参数名，参数值>
    QVariantMap args = data.toVariantMap();
    return invoke(uri, args);
}

QVariant McControllerContainer::invoke(const QString &uri, const QVariantList &data) noexcept
{
    QObjectPtr bean;
    QString func;
    QVariant errRet;
    if (!splitBeanAndFunc(uri, bean, func, errRet)) {
        return errRet;
    }
    return invokeForUri(bean, func, data);
}

QVariant McControllerContainer::invoke(const QString &uri, const QVariantMap &data) noexcept
{
    auto list = uri.split('?',
#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
                          Qt::SkipEmptyParts
#else
                          QString::SkipEmptyParts
#endif
    );
    if (list.isEmpty())
        return fail("access path not exists");
    QObjectPtr bean;
    QString func;
    QVariant errRet;
    if (!splitBeanAndFunc(list.at(0), bean, func, errRet)) {
        return errRet;
    }
    QString param = list.size() == 2 ? list.at(1) : "";
    // <参数名，参数值>
    QVariantMap args = splitParam(param);
    for (auto key : data.keys()) {
        args.insert(key, data.value(key));
    }
    return invokeForUri(bean, func, args);
}

bool McControllerContainer::splitBeanAndFunc(const QString &uri,
                                             QObjectPtr &bean,
                                             QString &func,
                                             QVariant &errRet) noexcept
{
    auto beanAndFunc = uri.split('.', 
#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
                                 Qt::SkipEmptyParts
#else
                                 QString::SkipEmptyParts
#endif
                                 );
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

QVariant McControllerContainer::invokeForUri(QObjectConstPtrRef bean,
                                             const QString &func,
                                             const QVariantMap &args) noexcept
{
    const QMetaObject *metaBean = bean->metaObject();
    auto params = args;
    int count = metaBean->methodCount();
    for (int i = 0; i < count; ++i) {
        QMetaMethod method = metaBean->method(i);
        if(method.name() != func)
            continue;
        if (!isMethodMatching(method, params.keys()))
            continue;
        if (!makeCallback(params, method)) {
            return fail("cannot construct callback function");
        }
        return invokeForArgs(bean, method, params);
    }
    return fail("access arguments error");
}

bool McControllerContainer::makeCallback(QVariantMap &args, const QMetaMethod &m) noexcept
{
    if (!args.contains(Mc::QuickBoot::Constant::Argument::qmlCallback)) {
        return true;
    }
    auto paramTypeNames = m.parameterTypes();
    if (paramTypeNames.isEmpty()) {
        return false;
    }
    auto qmlSyncCallbackVar = args.value(Mc::QuickBoot::Constant::Argument::qmlCallback);
    auto qmlSyncCallback = qmlSyncCallbackVar.value<McQmlSyncCallback *>();
    auto lastParamTypeName = paramTypeNames.last();
    auto lastParamType = QMetaType::type(lastParamTypeName);
    if (qMetaTypeId<McQmlSyncCallbackPtr>() == lastParamType
        || qMetaTypeId<McAbstractSyncCallbackPtr>() == lastParamType
        || qMetaTypeId<IMcCallbackPtr>() == lastParamType) {
        auto temp = McQmlSyncCallbackPtr(new McQmlSyncCallback(*qmlSyncCallback),
                                         IMcCallback::McCallbackDeleter());
        temp->moveToThread(qmlSyncCallback->thread());
        qmlSyncCallbackVar = QVariant::fromValue(temp);
    } else if (qMetaTypeId<McQmlSyncCallback *>() == lastParamType
               || qMetaTypeId<McAbstractSyncCallback *>() == lastParamType
               || qMetaTypeId<IMcCallback *>() == lastParamType) {
        auto temp = new McQmlSyncCallback(*qmlSyncCallback);
        temp->moveToThread(qmlSyncCallback->thread());
        qmlSyncCallbackVar = QVariant::fromValue(temp);
    } else if (qMetaTypeId<McQmlSyncCallback>() == lastParamType) {
        auto temp = *qmlSyncCallback;
        temp.moveToThread(qmlSyncCallback->thread());
        qmlSyncCallbackVar = QVariant::fromValue(temp);
    } else {
        return false;
    }
    args.remove(Mc::QuickBoot::Constant::Argument::qmlCallback);
    args.insert(m.parameterNames().last(), qmlSyncCallbackVar);
    return true;
}

QVariant McControllerContainer::invokeForUri(QObjectConstPtrRef bean,
                                             const QString &func,
                                             const QVariantList &args) noexcept
{
    const QMetaObject *metaBean = bean->metaObject();
    int count = metaBean->methodCount();
    for (int i = 0; i < count; ++i) {
        QMetaMethod method = metaBean->method(i);
        if (method.name() != func)
            continue;
        if (!isMethodMatching(method, args))
            continue;
        return invokeForArgs(bean, method, args);
    }
    return fail("access arguments error");
}

QVariantMap McControllerContainer::splitParam(const QString &param) noexcept
{
    QVariantMap args;
    QStringList params = param.split('&', 
#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
                                     Qt::SkipEmptyParts
#else
                                     QString::SkipEmptyParts
#endif
                                     );
    for (const auto &p : params) {
        QStringList nameAndValue = p.split('=', 
#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
                                           Qt::SkipEmptyParts
#else
                                           QString::SkipEmptyParts
#endif
                                           );
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

bool McControllerContainer::isMethodMatching(const QMetaMethod &m,
                                             const QList<QString> &names) noexcept
{
    QList<QByteArray> methodParamNames = m.parameterNames();
    if (methodParamNames.size() != names.size())
        return false;
    QList<QString> argNames = names;
    if (argNames.contains(Mc::QuickBoot::Constant::Argument::qmlCallback)) {
        argNames.removeOne(Mc::QuickBoot::Constant::Argument::qmlCallback);
        methodParamNames.removeLast();
    }
    removeDuplication(argNames, methodParamNames);
    if (methodParamNames.isEmpty())
        return true;
    else
        return false;
}

bool McControllerContainer::isMethodMatching(const QMetaMethod &m, const QVariantList &args) noexcept
{
    if (m.parameterCount() != args.length())
        return false;
    auto paramTypes = m.parameterTypes();
    for (int i = 0; i < paramTypes.length(); ++i) {
        auto paramType = paramTypes.at(i);
        auto arg = args.at(i);
        auto type = arg.typeName();
        if (paramType != type && !arg.canConvert(QMetaType::type(paramType))) {
            return false;
        }
    }
    return true;
}

void McControllerContainer::removeDuplication(QList<QString> &argNames,
                                              QList<QByteArray> &paramNames) noexcept
{
    for (int i = 0; i < paramNames.size(); ++i) {
        if (!argNames.contains(paramNames.at(i)))
            continue;
        argNames.removeOne(paramNames.at(i));
        paramNames.removeAt(i);
        --i;
    }
}

QVariant McControllerContainer::invokeForArgs(QObjectConstPtrRef bean,
                                              const QMetaMethod &method,
                                              const QVariantMap &args) noexcept
{
    auto returnType = method.returnType();
    if(returnType == QMetaType::Type::UnknownType) {
        qCritical() << "cannot found return type from meta object system. type name:"
                    << method.typeName();
        return fail("cannot found return type from meta object system");
    }
    QVariant returnValue;
    QGenericReturnArgument returnArg;
    if (method.returnType() == QMetaType::Void)
        returnValue = "调用成功";
    else {
        returnValue = QVariant(static_cast<QVariant::Type>(returnType));
        // 由于调用函数时需要对比类型名，所以这里类型名需要具体指定，不能再用QVariant
//        returnArg = Q_RETURN_ARG(QVariant, returnValue);
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
//        arguments.replace(i, Q_ARG(QVariant, values[i]));
        /* 由于业务需要，调用函数的方式为直接连接，通过源代码分析，直接连接的类型名
         * 不需要使用，只需要保证它不为空即可
         */
        arguments.replace(i, QGenericArgument("QVariant", values[i].data()));
    }
    if (!method.invoke(bean.data(),
                       Qt::DirectConnection,
                       returnArg,
                       arguments.at(0),
                       arguments.at(1),
                       arguments.at(2),
                       arguments.at(3),
                       arguments.at(4),
                       arguments.at(5),
                       arguments.at(6),
                       arguments.at(7),
                       arguments.at(8),
                       arguments.at(9))) {
        return fail("failed invoke function");
    }
    return returnValue;
}

QVariant McControllerContainer::invokeForArgs(QObjectConstPtrRef bean,
                                              const QMetaMethod &method,
                                              const QVariantList &args) noexcept
{
    auto returnType = method.returnType();
    if (returnType == QMetaType::Type::UnknownType) {
        qCritical() << "cannot found return type from meta object system. type name:"
                    << method.typeName();
        return fail("cannot found return type from meta object system");
    }
    QVariant returnValue;
    QGenericReturnArgument returnArg;
    if (method.returnType() == QMetaType::Void)
        returnValue = "调用成功";
    else {
        returnValue = QVariant(static_cast<QVariant::Type>(returnType));
        // 由于调用函数时需要对比类型名，所以这里类型名需要具体指定，不能再用QVariant
        //        returnArg = Q_RETURN_ARG(QVariant, returnValue);
        returnArg = QGenericReturnArgument(method.typeName(), returnValue.data());
    }
    QVector<QGenericArgument> arguments;
    arguments.resize(10);

    bool ok = false;
    QVariant errMsg;
    auto values = makeValues(method, args, arguments.size(), &errMsg, &ok);
    if (!ok) {
        return errMsg;
    }
    for (int i = 0; i < values.size(); ++i) {
        // 由于Q_ARG宏所调用的函数是直接取value的地址，所以value不能是局部变量
        //        arguments.replace(i, Q_ARG(QVariant, values[i]));
        /* 由于业务需要，调用函数的方式为直接连接，通过源代码分析，直接连接的类型名
         * 不需要使用，只需要保证它不为空即可
         */
        arguments.replace(i, QGenericArgument("QVariant", values[i].data()));
    }
    if (!method.invoke(bean.data(),
                       Qt::DirectConnection,
                       returnArg,
                       arguments.at(0),
                       arguments.at(1),
                       arguments.at(2),
                       arguments.at(3),
                       arguments.at(4),
                       arguments.at(5),
                       arguments.at(6),
                       arguments.at(7),
                       arguments.at(8),
                       arguments.at(9))) {
        return fail("failed invoke function");
    }
    return returnValue;
}

QVariantList McControllerContainer::makeValues(const QMetaMethod &method,
                                               const QVariantMap &args,
                                               int maxParamSize,
                                               QVariant *errMsg,
                                               bool *ok) noexcept
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
        list.append(McJsonUtils::fromJson(type, value));
    }
    *ok = true;
    return list;
}

QVariantList McControllerContainer::makeValues(const QMetaMethod &method,
                                               const QVariantList &args,
                                               int maxParamSize,
                                               QVariant *errMsg,
                                               bool *ok) noexcept
{
    bool isOk = false;
    QVariant msg;
    if (ok == nullptr) {
        ok = &isOk;
    }
    if (errMsg == nullptr) {
        errMsg = &msg;
    }
    QList<QByteArray> paramTypes = method.parameterTypes();
    if (paramTypes.size() > maxParamSize) {
        *errMsg = fail(QString("argument for function cannot more than %1").arg(maxParamSize));
        *ok = false;
        return QVariantList();
    }
    QVariantList list;
    for (int i = 0; i < paramTypes.size(); ++i) {
        QByteArray type = paramTypes.at(i).simplified();
        QVariant value = args.at(i); //!< 由于调用此函数时参数个数一定一致，所以这里一定有值
        if (!value.convert(QMetaType::type(type))) {
            qCritical() << "property convert failure. origin type name:" << value.typeName()
                        << "typeid:" << value.type() << "target typeName:" << type;
            *errMsg = fail("cannot convert property");
            *ok = false;
            return QVariantList();
        }
        list.append(value);
    }
    *ok = true;
    return list;
}

QVariant McControllerContainer::ok(const QVariant &val) const noexcept 
{
    auto result = McResult::ok(val);
    return QVariant::fromValue(result);
}

QVariant McControllerContainer::fail(const QString &val) const noexcept 
{
    auto result = McResult::fail(val);
    return QVariant::fromValue(result);
}
