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
#include "McAbstractInvoker.h"

#include <McCore/Utils/McRequest.h>
#include <McCore/Utils/McSlotObjectWrapper.h>
#include <McIoc/ApplicationContext/IMcApplicationContext.h>

#include "ComponentContainer/McControllerContainer.h"
#include "McInvoker.h"
#include "McQuickBootGlobal.h"

MC_DECL_PRIVATE_DATA2(McAbstractInvoker)
{
    IMcApplicationContext *appCtx{nullptr};
    McControllerContainerPtr controllerContainer;
};

McAbstractInvoker::McAbstractInvoker(QObject *parent) noexcept
    : QObject(parent)
{
    MC_NEW_PRIVATE_DATA(McAbstractInvoker);
}

McAbstractInvoker::~McAbstractInvoker() {}

IMcApplicationContext *McAbstractInvoker::appCtx() const noexcept
{
    return d->appCtx;
}

void McAbstractInvoker::setAppCtx(IMcApplicationContext *val) noexcept
{
    d->appCtx = val;
}

QObject *McAbstractInvoker::getBean(const QString &name) const noexcept
{
    auto var = getBeanToVariant(name);
    QObject *obj = var.value<QObject *>();
    if (obj == nullptr) {
        obj = var.value<QObjectPtr>().data();
    }
    if (obj == nullptr) {
        qCWarning(mcQuickBoot) << "cannot get bean for named:" << name;
    } else {
        //#ifndef MC_TINY_QUICK_BOOT
        //        QQmlEngine::setObjectOwnership(obj, QQmlEngine::CppOwnership);
        //#endif
    }
    return obj;
}

// QObject *McAbstractInvoker::getModel(const QString &name) const noexcept
//{
//     return d->modelContainer->getModel(name);
// }

void McAbstractInvoker::run(
    McAbstractPromise *promise, const QString &uri, const QVariantList &data, bool async) noexcept
{
    auto controller = d->controllerContainer->find(uri, data);
    if (!controller.has_value()) {
        promise->setBody(McPrivate::fail(QString("Cannot find controller: '%s'").arg(uri)));
        return;
    }
    auto functor = McSlotObjectWrapper::build(nullptr, [uri, controller](const McRequest &req) -> QVariant {
        if (req.count() > 10) {
            return McPrivate::fail(QString("Argument for function cannot more than %1").arg(10));
        }

        QVariant returnValue;
        QGenericReturnArgument returnArg;
#ifdef MC_USE_QT5
        if (controller->returnMetaType != QMetaType::Void) {
            returnValue = QVariant(static_cast<QVariant::Type>(controller->returnMetaType));
            returnArg = QGenericReturnArgument(controller->method.typeName(), returnValue.data());
        }
#else
        if (controller->returnMetaType != QMetaType::fromType<void>()) {
            returnValue = QVariant(controller->returnMetaType);
            // 由于调用函数时需要对比类型名，所以这里类型名需要具体指定，不能再用QVariant
            //        returnArg = Q_RETURN_ARG(QVariant, returnValue);
            returnArg = QGenericReturnArgument(controller->returnMetaType.name(), returnValue.data());
        }
#endif
        else {
            returnValue = "call successful";
        }

        QVariantList values = req.params();
        QVector<QGenericArgument> arguments;
        arguments.resize(10);

        for (int i = 0; i < values.size(); ++i) {
            // 由于Q_ARG宏所调用的函数是直接取value的地址，所以value不能是局部变量
            //        arguments.replace(i, Q_ARG(QVariant, values[i]));
            /* 由于业务需要，调用函数的方式为直接连接，通过源代码分析，直接连接的类型名
             * 不需要使用，只需要保证它不为空即可
             */
            arguments.replace(i, QGenericArgument("QVariant", values[i].data()));
        }
        if (!controller->method.invoke(controller->bean.data(), Qt::DirectConnection, returnArg, arguments.at(0),
                arguments.at(1), arguments.at(2), arguments.at(3), arguments.at(4), arguments.at(5), arguments.at(6),
                arguments.at(7), arguments.at(8), arguments.at(9))) {
            return McPrivate::fail(QString("Failed invoke function: '%s'").arg(uri));
        }
        return returnValue;
    });
    if (async) {
        McPrivate::invoke(promise, functor, controller->parameterMetaTypes, data);
    } else {
        promise->setBody(McPrivate::syncInvoke(functor, controller->parameterMetaTypes, data));
    }
}

QVariant McAbstractInvoker::getBeanToVariant(const QString &name) const noexcept
{
    return d->appCtx->getBean(name);
}

#include "moc_McAbstractInvoker.cpp"