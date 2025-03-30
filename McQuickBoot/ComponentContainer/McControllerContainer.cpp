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
#include "McControllerContainer.h"

#include <McCore/Utils/McJsonUtils.h>
#include <McCore/Utils/McRequest.h>
#include <McIoc/ApplicationContext/IMcApplicationContext.h>

#include "McQuickBoot/Utils/McQmlSyncCallback.h"

namespace {
struct ControllerData
{
    struct Method
    {
        bool isRequest{false}; ///< 参数是否是McRequest
        bool isCustomRequest{false}; ///< 参数是否是McCustomRequest
        QMetaMethod method;
        QList<QByteArray> parameterNames;
#ifdef MC_USE_QT5
        QList<int> parameterMetaTypes;
        int returnMetaType;
#else
        QList<QMetaType> parameterMetaTypes;
        QMetaType returnMetaType;
#endif
    };

    QString beanName;
    QObjectPtr bean;
    //! <funcName, Method>
    QMap<QString, QList<Method>> methods;
};
} // namespace

MC_AUTO_INIT2(McControllerContainer) {}

MC_DECL_PRIVATE_DATA2(McControllerContainer)
{
    //! <beanName, ControllerData>
    QMap<QString, ControllerData> controllers;
};

McControllerContainer::McControllerContainer(QObject *parent) noexcept
    : QObject(parent)
{
    MC_NEW_PRIVATE_DATA(McControllerContainer);
}

McControllerContainer::~McControllerContainer() {}

void McControllerContainer::init(const IMcApplicationContextPtr &appCtx) noexcept
{
    d->controllers.clear();
    auto beanNames = Mc::getComponents(appCtx, QStringLiteral(MC_CONTROLLER_TAG));
    QStringList useRequestMethods; //!< 使用了McRequest的方法，此类方法不能使用重载
    for (const auto &beanName : std::as_const(beanNames)) {
        auto obj = appCtx->getBean<QObjectPtr>(beanName);
        if (obj.isNull()) {
            qCCritical(mcQuickBoot, "controller for named '%s' not exists", qPrintable(beanName));
            continue;
        }
        if (d->controllers.contains(beanName)) {
            qCCritical(mcQuickBoot, "controller for named '%s' is repeated", qPrintable(beanName));
            continue;
        }
        ControllerData controller;
        controller.beanName = beanName;
        controller.bean = obj;
        const QMetaObject *metaObj = obj->metaObject();
        int count = metaObj->methodCount();
#ifdef MC_USE_QT6
        useRequestMethods.resize(0);
#endif
        for (int i = 0; i < count; ++i) {
            QMetaMethod method = metaObj->method(i);
            if (useRequestMethods.contains(method.name())) {
                qCWarning(mcQuickBoot, "the function cannot overload when McRequest/McCustomRequest is used");
                continue;
            }
            ControllerData::Method m;
            m.method = method;
            m.parameterNames = method.parameterNames();
#ifdef MC_USE_QT5
            m.returnMetaType = method.returnType();
#else
            m.returnMetaType = method.returnMetaType();
#endif
            for (int j = 0; j < method.parameterCount(); ++j) {
#ifdef MC_USE_QT5
                auto type = method.parameterType(j);
                m.isRequest = (type == qMetaTypeId<McRequest>());
#else
                auto type = method.parameterMetaType(j);
                m.isRequest = (type == QMetaType::fromType<McRequest>());
#endif
                if (!m.isRequest) {
                    m.isCustomRequest = McPrivate::isContainedCustomRequest(type);
                }
                if (m.isRequest || m.isCustomRequest) {
                    Q_ASSERT_X(m.parameterMetaTypes.isEmpty() && method.parameterCount() == 1, "Parse Controller",
                        "The function cannot have multiple parameters when McRequest/McCustomRequest is used");
                    m.parameterMetaTypes = {type};
                    break;
                }

                m.parameterMetaTypes.append(type);
            }
            auto &methods = controller.methods[method.name()];
            if (m.isRequest || m.isCustomRequest) {
                if (!methods.isEmpty()) {
                    qCWarning(mcQuickBoot, "the function cannot overload when McRequest/McCustomRequest is used");
                }
                methods = {m};
                useRequestMethods.append(method.name());
                continue;
            }
            methods.append(m);
        }
        d->controllers.insert(beanName, controller);
    }
}

std::optional<McControllerEntity> McControllerContainer::find(const QString &uri) const noexcept
{
    auto uriList = uri.split('?', Qt::SkipEmptyParts);
    if (uriList.isEmpty()) {
        return std::nullopt;
    }

    if (uriList.size() == 1) {
        return find(uriList.at(0), QVariantList());
    } else {
        QVariantMap args;
        QStringList params = uriList.at(1).split('&', Qt::SkipEmptyParts);
        for (const auto &p : params) {
            QStringList nameAndValue = p.split('=', Qt::SkipEmptyParts);
            if (nameAndValue.isEmpty())
                continue;
            QString key = nameAndValue.at(0);
            QVariant value;
            if (nameAndValue.size() == 2)
                value = nameAndValue.at(1);
            else
                value = "";
            args.insert(key, value);
        }
        return find(uriList.at(0), args);
    }
}

std::optional<McControllerEntity> McControllerContainer::find(
    const QString &uri, const QVariantList &data) const noexcept
{
    McControllerEntity controller;
    QString beanName;
    QString funcName;
    if (!splitBeanAndFunc(uri, beanName, funcName)) {
        return std::nullopt;
    }
    if (!d->controllers.contains(beanName)) {
        return std::nullopt;
    }
    const auto &con = d->controllers.value(beanName);
    controller.bean = con.bean;
    if (!con.methods.contains(funcName)) {
        return std::nullopt;
    }
    const auto &ms = con.methods.value(funcName);
    for (auto &m : ms) {
        bool isFound = false;
        if (m.isRequest) {
            controller.arguments = data;
            isFound = true;
        } else {
            auto parameterMetaTypes = m.parameterMetaTypes;
            if (m.isCustomRequest) {
                auto reqType = m.parameterMetaTypes.at(0);
                parameterMetaTypes = McPrivate::getCustomRequestMetaType(reqType);
            }
            if (parameterMetaTypes.size() == data.size()) {
                for (int i = 0; i < parameterMetaTypes.size(); ++i) {
                    auto metaType = parameterMetaTypes.at(i);
                    auto arg = data.at(i);
#ifdef MC_USE_QT5
                    auto type = arg.userType();
                    if (type == qMetaTypeId<McQmlSyncCallbackPtr>()) {
#else
                    auto type = arg.metaType();
                    if (type == QMetaType::fromType<McQmlSyncCallbackPtr>()) {
#endif
                        QVariant callbackVar = maybeBuildCallback(arg, metaType);
                        if (callbackVar.isValid()) {
                            controller.arguments.append(callbackVar);
                            continue;
                        }
                    }
                    controller.arguments.append(McJsonUtils::deserialize(arg, metaType));
                }
                isFound = true;
            }
        }
        if (!isFound) {
            continue;
        }
        controller.method = m.method;
        controller.parameterMetaTypes = m.parameterMetaTypes;
        controller.returnMetaType = m.returnMetaType;
        return controller;
    }
    return std::nullopt;
}

std::optional<McControllerEntity> McControllerContainer::find(const QString &uri, const QVariantMap &data) const noexcept
{
    McControllerEntity controller;
    QString beanName;
    QString funcName;
    if (!splitBeanAndFunc(uri, beanName, funcName)) {
        return std::nullopt;
    }
    if (!d->controllers.contains(beanName)) {
        return std::nullopt;
    }
    const auto &con = d->controllers.value(beanName);
    controller.bean = con.bean;
    if (!con.methods.contains(funcName)) {
        return std::nullopt;
    }
    const auto &ms = con.methods.value(funcName);
    const QStringList &argNames = data.keys();
    for (auto &m : ms) {
        bool isFound = false;
        if (m.isRequest) {
            controller.arguments = data.values();
            isFound = true;
        } else if (m.isCustomRequest) {
            auto reqType = m.parameterMetaTypes.at(0);
            auto childMetaTypes = McPrivate::getCustomRequestMetaType(reqType);
            if (childMetaTypes.size() == data.size()) {
                QVariantList args = data.values();
                for (int i = 0; i < childMetaTypes.size(); ++i) {
                    auto metaType = childMetaTypes.at(i);
                    auto arg = args.at(i);
#ifdef MC_USE_QT5
                    auto type = arg.userType();
                    if (type == qMetaTypeId<McQmlSyncCallbackPtr>()) {
#else
                    auto type = arg.metaType();
                    if (type == QMetaType::fromType<McQmlSyncCallbackPtr>()) {
#endif
                        QVariant callbackVar = maybeBuildCallback(arg, metaType);
                        if (callbackVar.isValid()) {
                            controller.arguments.append(callbackVar);
                            continue;
                        }
                    }
                    controller.arguments.append(McJsonUtils::deserialize(arg, metaType));
                }
                isFound = true;
            }
        } else {
            QList<QByteArray> methodParamNames = m.method.parameterNames();
            if (methodParamNames.size() == data.size()) {
                bool hasCallback = argNames.contains(Mc::QuickBoot::Constant::Argument::qmlCallback);
                for (int i = 0; i < m.parameterMetaTypes.size(); ++i) {
                    auto metaType = m.parameterMetaTypes.at(i);
                    if (hasCallback) {
                        auto qmlSyncCallbackVar = data.value(Mc::QuickBoot::Constant::Argument::qmlCallback);
                        QVariant callbackVar = maybeBuildCallback(qmlSyncCallbackVar, metaType);
                        if (callbackVar.isValid()) {
                            controller.arguments.append(callbackVar);
                            continue;
                        }
                    }
                    QByteArray name = methodParamNames.at(i).simplified();
                    QVariant value = data[name]; //!< 由于调用此函数时参数名一定存在，所以这里一定有值
                    controller.arguments.append(McJsonUtils::deserialize(value, metaType));
                }
                isFound = true;
            }
        }
        if (!isFound) {
            continue;
        }
        controller.method = m.method;
        controller.parameterMetaTypes = m.parameterMetaTypes;
        controller.returnMetaType = m.returnMetaType;
        return controller;
    }
    return std::nullopt;
}

bool McControllerContainer::splitBeanAndFunc(const QString &uri, QString &beanName, QString &funcName) const noexcept
{
    QStringList beanAndFunc = uri.split('.', Qt::SkipEmptyParts);
    if (beanAndFunc.size() != 2) {
        return false;
    }
    beanName = beanAndFunc.at(0);
    funcName = beanAndFunc.at(1);
    return true;
}

#ifdef MC_USE_QT5
QVariant McControllerContainer::maybeBuildCallback(const QVariant &originVar, int metaType) const noexcept
#else
QVariant McControllerContainer::maybeBuildCallback(const QVariant &originVar, const QMetaType &metaType) const noexcept
#endif
{
    auto qmlSyncCallback = originVar.value<McQmlSyncCallbackPtr>();
#ifdef MC_USE_QT5
    if (metaType == qMetaTypeId<McQmlSyncCallbackPtr>() || metaType == qMetaTypeId<McAbstractSyncCallbackPtr>()
        || metaType == qMetaTypeId<IMcCallbackPtr>()) {
#else
    if (metaType == QMetaType::fromType<McQmlSyncCallbackPtr>()
        || metaType == QMetaType::fromType<McAbstractSyncCallbackPtr>()
        || metaType == QMetaType::fromType<IMcCallbackPtr>()) {
#endif
        return originVar;
#ifdef MC_USE_QT5
    } else if (metaType == qMetaTypeId<McQmlSyncCallback *>() || metaType == qMetaTypeId<McAbstractSyncCallback *>()
               || metaType == qMetaTypeId<IMcCallback *>()) {
#else
    } else if (metaType == QMetaType::fromType<McQmlSyncCallback *>()
               || metaType == QMetaType::fromType<McAbstractSyncCallback *>()
               || metaType == QMetaType::fromType<IMcCallback *>()) {
#endif
        auto temp = new McQmlSyncCallback(*qmlSyncCallback);
        temp->moveToThread(qmlSyncCallback->thread());
        return QVariant::fromValue(temp);
#ifdef MC_USE_QT5
    } else if (metaType == qMetaTypeId<McQmlSyncCallback>()) {
#else
    } else if (metaType == QMetaType::fromType<McQmlSyncCallback>()) {
#endif
        auto temp = *qmlSyncCallback;
        temp.moveToThread(qmlSyncCallback->thread());
        return QVariant::fromValue(temp);
    } else {
        return QVariant();
    }
}
