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
#include "McQmlInvoker.h"

#include <QQmlEngine>

#include "McCppPromise.h"
#include "McQmlPromise.h"
#include "Utils/McQmlSyncCallback.h"

MC_AUTO_INIT2(McQmlInvoker) {}

McQmlInvoker::McQmlInvoker(QObject *parent) noexcept
    : McAbstractInvoker(parent)
{
}

McQmlInvoker::~McQmlInvoker() {}

McQmlPromise *McQmlInvoker::invoke(const QString &uri, const QJSValue &data1, const QJSValue &data2) noexcept
{
    if (!data2.isUndefined() && !data2.isCallable()) {
        qCritical() << "The third parameter for invoke method must be callback function";
        return nullptr;
    }
#if QT_VERSION < QT_VERSION_CHECK(5, 5, 0)
    auto engine = QQmlEngine::contextForObject(this)->engine();
#else
    auto engine = qjsEngine(this);
#endif
    if (!engine) {
        return nullptr;
    }
    auto promise = new McQmlPromise(); //!< 没有指定父对象，该对象将在整个请求完毕时被析构
    QQmlEngine::setObjectOwnership(promise, QQmlEngine::CppOwnership);
    if (data1.isUndefined()) {
        run(promise, uri);
        return promise;
    }
    if (data1.isCallable()) {
        QVariantMap m;
        m.insert(Mc::QuickBoot::Constant::Argument::qmlCallback,
            QVariant::fromValue(McQmlSyncCallback::build(data1, engine)));
        run(promise, uri, m);
        return promise;
    }
    if (data1.isArray()) {
        auto list = data1.toVariant().toList();
        if (!data2.isUndefined()) {
            list.append(QVariant::fromValue(McQmlSyncCallback::build(data2, engine)));
        }
        run(promise, uri, list);
    } else {
        auto map = data1.toVariant().toMap();
        if (!data2.isUndefined()) {
            map.insert(Mc::QuickBoot::Constant::Argument::qmlCallback,
                QVariant::fromValue(McQmlSyncCallback::build(data2, engine)));
        }
        run(promise, uri, map);
    }
    return promise;
}

QJSValue McQmlInvoker::syncInvoke(const QString &uri, const QJSValue &data) noexcept
{
    auto promise = new McCppPromise(); //!< 没有指定父对象，该对象将在整个请求完毕时被析构
    QVariant result;
    promise->asyncThen([&result](const QVariant &var) { result = var; });
    if (data.isUndefined()) {
        run(promise, uri, false);
    } else if (data.isArray()) {
        run(promise, uri, data.toVariant().toList(), false);
    } else {
        run(promise, uri, data.toVariant().toMap(), false);
    }

#if QT_VERSION < QT_VERSION_CHECK(5, 5, 0)
    auto engine = QQmlEngine::contextForObject(this)->engine();
#else
    auto engine = qjsEngine(this);
#endif
    if (!engine) {
        return QJSValue();
    }
    auto arg = engine->toScriptValue(result);
    return arg;
}
