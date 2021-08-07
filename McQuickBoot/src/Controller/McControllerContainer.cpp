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
#include "McBoot/Requestor/McRequest.h"
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
        if (d->controllers.contains(beanName)) {
            qCritical() << QString("controller for named '%1' is repeated").arg(beanName);
            continue;
        }
        d->controllers.insert(beanName, obj);
    }
}

QVariant McControllerContainer::invoke(const QString &uri,
                                       const QVariant &body,
                                       const McRequest &request) noexcept
{
    switch (static_cast<QMetaType::Type>(body.type())) {
    case QMetaType::Type::UnknownType:
        return invoke(uri, request);
    case QMetaType::Type::QJsonObject:
        return invoke(uri, body.toJsonObject(), request);
    case QMetaType::Type::QVariantList:
        return invoke(uri, body.toList(), request);
    case QMetaType::Type::QVariantMap:
        return invoke(uri, body.toMap(), request);
    default:
        return fail(
            QString("Uri:'%1' access failure. this body is not support type. type:%2. typeName:%3")
                .arg(uri, QString::number(body.type()), body.typeName()));
    }
}

QVariant McControllerContainer::invoke(const QString &uri, const McRequest &request) noexcept
{
    return invoke(uri, QVariantMap(), request);
}

QVariant McControllerContainer::invoke(const QString &uri,
                                       const QJsonObject &data,
                                       const McRequest &request) noexcept
{
    // <参数名，参数值>
    QVariantMap args = data.toVariantMap();
    return invoke(uri, args, request);
}

QVariant McControllerContainer::invoke(const QString &uri,
                                       const QVariantList &data,
                                       const McRequest &request) noexcept
{
    QObjectPtr bean;
    QString func;
    QVariant ret;
    if (splitBeanAndFunc(uri, bean, func, ret)) {
        ret = invokeForUri(bean, func, data, request);
    }
    if (ret.canConvert<McResultPtr>()) {
        auto result = ret.value<McResultPtr>();
        result->setErrMsg(QString("Uri:'%1' access failure. ").arg(uri) + result->errMsg());
    }
    return ret;
}

QVariant McControllerContainer::invoke(const QString &uri,
                                       const QVariantMap &data,
                                       const McRequest &request) noexcept
{
    auto list = uri.split('?',
#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
                          Qt::SkipEmptyParts
#else
                          QString::SkipEmptyParts
#endif
    );
    QVariant ret;
    if (list.isEmpty()) {
        ret = fail("access path not exists");
    } else {
        QObjectPtr bean;
        QString func;
        if (splitBeanAndFunc(list.at(0), bean, func, ret)) {
            QString param = list.size() == 2 ? list.at(1) : "";
            // <参数名，参数值>
            QVariantMap args = splitParam(param);
            auto keys = data.keys();
            for (const auto &key : keys) {
                args.insert(key, data.value(key));
            }
            ret = invokeForUri(bean, func, args, request);
        }
    }
    if (ret.canConvert<McResultPtr>()) {
        auto result = ret.value<McResultPtr>();
        result->setErrMsg(QString("Uri:'%1' access failure. ").arg(uri) + result->errMsg());
    }
    return ret;
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
                                             const QVariantMap &args,
                                             const McRequest &request) noexcept
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
        return invokeForArgs(bean, method, params, request);
    }
    return fail("no matching method");
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
    auto qmlSyncCallback = qmlSyncCallbackVar.value<McQmlSyncCallbackPtr>();
    auto lastParamTypeName = paramTypeNames.last();
    auto lastParamType = QMetaType::type(lastParamTypeName);
    if (qMetaTypeId<McQmlSyncCallbackPtr>() == lastParamType
        || qMetaTypeId<McAbstractSyncCallbackPtr>() == lastParamType
        || qMetaTypeId<IMcCallbackPtr>() == lastParamType) {
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
    args.insert(m.parameterNames().constLast(), qmlSyncCallbackVar);
    return true;
}

QVariant McControllerContainer::invokeForUri(QObjectConstPtrRef bean,
                                             const QString &func,
                                             const QVariantList &args,
                                             const McRequest &request) noexcept
{
    const QMetaObject *metaBean = bean->metaObject();
    int count = metaBean->methodCount();
    for (int i = 0; i < count; ++i) {
        QMetaMethod method = metaBean->method(i);
        if (method.name() != func)
            continue;
        if (!isMethodMatching(method, args))
            continue;
        return invokeForArgs(bean, method, args, request);
    }
    return fail("no matching method");
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
    using namespace Mc::QuickBoot::Private;
    if (m.parameterCount() == 1) {
        auto id = m.parameterType(0);
        if (id == qMetaTypeId<McRequest>()) {
            return true;
        }
        if (isContainedCustomRequest(id)) {
            auto childrenId = getCustomRequestId(id);
            if (childrenId.size() == names.size()) {
                return true;
            } else {
                return false;
            }
        }
    }
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
    using namespace Mc::QuickBoot::Private;
    auto paramTypes = m.parameterTypes();
    if (m.parameterCount() == 1) {
        auto id = m.parameterType(0);
        if (id == qMetaTypeId<McRequest>()) {
            return true;
        }
        if (isContainedCustomRequest(id)) {
            auto childrenId = getCustomRequestId(id);
            paramTypes.clear();
            for (auto childId : childrenId) {
                paramTypes.append(QMetaType::typeName(childId));
            }
        }
    }
    if (paramTypes.size() != args.size())
        return false;
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
                                              const QVariantMap &args,
                                              const McRequest &request) noexcept
{
    auto returnType = method.returnType();
    if(returnType == QMetaType::Type::UnknownType) {
        auto result = fail(QString("cannot found return type from meta object system. type name:%1")
                               .arg(method.typeName()));
        return result;
    }
    QVariant returnValue;
    QGenericReturnArgument returnArg;
    if (method.returnType() == QMetaType::Void)
        returnValue = "call successful";
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
    auto values = makeValues(method, args, arguments.size(), request, &errMsg, &ok);
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
                                              const QVariantList &args,
                                              const McRequest &request) noexcept
{
    auto returnType = method.returnType();
    if (returnType == QMetaType::Type::UnknownType) {
        auto result = fail(
            QString("cannot found return type from meta object system. type name: %1")
                .arg(method.typeName()));
        return result;
    }
    QVariant returnValue;
    QGenericReturnArgument returnArg;
    if (method.returnType() == QMetaType::Void)
        returnValue = "call successful";
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
    auto values = makeValues(method, args, arguments.size(), request, &errMsg, &ok);
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
                                               const McRequest &request,
                                               QVariant *errMsg,
                                               bool *ok) noexcept
{
    using namespace Mc::QuickBoot::Private;
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
    if (paramTypes.size() == 1 && method.parameterType(0) == qMetaTypeId<McRequest>()) {
        McRequest req = request;
        req.setParams(args.values());
        list.append(QVariant::fromValue(req));
    } else if (paramTypes.size() == 1 && isContainedCustomRequest(method.parameterType(0))) {
        auto mainId = method.parameterType(0);
        auto childrenId = getCustomRequestId(mainId);
        QVariantList reqArgs;
        for (int i = 0; i < childrenId.size(); ++i) {
            auto type = childrenId.at(i);
            QByteArray name = paramNames.at(i).simplified();
            QVariant value = args[name]; //!< 由于调用此函数时参数名一定存在，所以这里一定有值
            reqArgs.append(McJsonUtils::fromJson(type, value));
        }
        list.append(buildCustomRequest(mainId, reqArgs, request));
    } else {
        for (int i = 0; i < paramTypes.size(); ++i) {
            QByteArray type = paramTypes.at(i).simplified();
            QByteArray name = paramNames.at(i).simplified();
            QVariant value = args[name]; //!< 由于调用此函数时参数名一定存在，所以这里一定有值
            list.append(McJsonUtils::fromJson(type, value));
        }
    }
    *ok = true;
    return list;
}

QVariantList McControllerContainer::makeValues(const QMetaMethod &method,
                                               const QVariantList &args,
                                               int maxParamSize,
                                               const McRequest &request,
                                               QVariant *errMsg,
                                               bool *ok) noexcept
{
    using namespace Mc::QuickBoot::Private;
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
    if (paramTypes.size() == 1 && method.parameterType(0) == qMetaTypeId<McRequest>()) {
        McRequest req = request;
        req.setParams(args);
        list.append(QVariant::fromValue(req));
    } else if (paramTypes.size() == 1 && isContainedCustomRequest(method.parameterType(0))) {
        auto mainId = method.parameterType(0);
        auto childrenId = getCustomRequestId(mainId);
        QVariantList reqArgs;
        for (int i = 0; i < childrenId.size(); ++i) {
            auto type = childrenId.at(i);
            QVariant value = args.at(i); //!< 由于调用此函数时参数个数一定一致，所以这里一定有值
            if (!value.convert(type)) {
                *errMsg = fail(QString("property convert failure. origin type name:%1. typeid:%2. "
                                       "target typeName:%3")
                                   .arg(value.typeName(),
                                        QString::number(value.type()),
                                        QMetaType::typeName(type)));
                *ok = false;
                return QVariantList();
            }
            reqArgs.append(value);
        }
        list.append(buildCustomRequest(mainId, reqArgs, request));
    } else {
        for (int i = 0; i < paramTypes.size(); ++i) {
            QByteArray type = paramTypes.at(i).simplified();
            QVariant value = args.at(i); //!< 由于调用此函数时参数个数一定一致，所以这里一定有值
            if (!value.convert(QMetaType::type(type))) {
                *errMsg = fail(QString("property convert failure. origin type name:%1. typeid:%2. "
                                       "target typeName:%3")
                                   .arg(value.typeName(), QString::number(value.type()), type));
                *ok = false;
                return QVariantList();
            }
            list.append(value);
        }
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
    result->setInternalError(true);
    return QVariant::fromValue(result);
}
