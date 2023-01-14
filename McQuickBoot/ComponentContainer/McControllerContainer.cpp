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

#include <McCore/Utils/McRequest.h>
#include <McIoc/ApplicationContext/IMcApplicationContext.h>

namespace {
struct ControllerData
{
    struct Method
    {
        bool isRequest{false};
        QMetaMethod method;
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
    for (const auto &beanName : beanNames) {
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
        useRequestMethods.resize(0);
        for (int i = 0; i < count; ++i) {
            QMetaMethod method = metaObj->method(i);
            if (useRequestMethods.contains(method.name())) {
                qCWarning(mcQuickBoot, "the function cannot overload when McRequest/McCustomRequest is used");
                continue;
            }
            ControllerData::Method m;
            m.method = method;
#ifdef MC_USE_QT5
            m.returnMetaType = method.returnType();
#else
            m.returnMetaType = method.returnMetaType();
#endif
            bool findRequest = false;
            for (int j = 0; j < method.parameterCount(); ++j) {
#ifdef MC_USE_QT5
                auto type = method.parameterType(j);
                if (type == qMetaTypeId<McRequest>() || McPrivate::isContainedCustomRequest(type)) {
                    findRequest = true;
                    Q_ASSERT_X(m.parameterMetaTypes.isEmpty() && method.parameterCount() == 1, "Parse Controller",
                        "The function cannot have multiple parameters when McRequest/McCustomRequest is used");
                    m.parameterMetaTypes = {type};
                    break;
                }
                m.parameterMetaTypes.append(type);
#else
                auto type = method.parameterMetaType(j);
                if (type == QMetaType::fromType<McRequest>() || McPrivate::isContainedCustomRequest(type)) {
                    findRequest = true;
                    Q_ASSERT_X(m.parameterMetaTypes.isEmpty() && method.parameterCount() == 1, "Parse Controller",
                        "The function cannot have multiple parameters when McRequest/McCustomRequest is used");
                    m.parameterMetaTypes = {type};
                    break;
                }
                m.parameterMetaTypes.append(type);
#endif
            }
            auto &methods = controller.methods[method.name()];
            if (findRequest) {
                if (!methods.isEmpty()) {
                    qCWarning(mcQuickBoot, "the function cannot overload when McRequest/McCustomRequest is used");
                }
                m.isRequest = true;
                methods = {m};
                useRequestMethods.append(method.name());
                continue;
            }
            methods.append(m);
        }
        d->controllers.insert(beanName, controller);
    }
}

std::optional<McControllerContainer::Controller> McControllerContainer::find(
    const QString &uri, const QVariantList &data) const noexcept
{
    Controller controller;
    QStringList beanAndFunc = uri.split('.', Qt::SkipEmptyParts);
    if (beanAndFunc.size() != 2) {
        return std::nullopt;
    }
    if (!d->controllers.contains(beanAndFunc.at(0))) {
        return std::nullopt;
    }
    const auto &con = d->controllers.value(beanAndFunc.at(0));
    controller.bean = con.bean;
    if (!con.methods.contains(beanAndFunc.at(1))) {
        return std::nullopt;
    }
    auto evalFunc = [&controller](const ControllerData::Method &m) {
        controller.method = m.method;
        controller.parameterMetaTypes = m.parameterMetaTypes;
        controller.returnMetaType = m.returnMetaType;
    };
    const auto &ms = con.methods.value(beanAndFunc.at(1));
    if (ms.size() == 1) {
        evalFunc(ms.constFirst());
    } else {
        bool find = false;
        for (auto &m : ms) {
            if (m.parameterMetaTypes.size() != data.size()) {
                continue;
            }
            bool flag = true;
            for (int i = 0; i < m.parameterMetaTypes.size(); ++i) {
                auto paramType = m.parameterMetaTypes.at(i);
                auto arg = data.at(i);
#ifdef MC_USE_QT5
                auto type = arg.userType();
#else
                auto type = arg.metaType();
#endif
                if (paramType != type && !arg.canConvert(paramType)) {
                    flag = false;
                    break;
                }
            }
            if (!flag) {
                continue;
            }
            find = true;
            evalFunc(m);
            break;
        }
        if (!find) {
            return std::nullopt;
        }
    }
    return std::make_optional(controller);
}
