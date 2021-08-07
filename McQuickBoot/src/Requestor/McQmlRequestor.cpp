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
#include "McBoot/Requestor/McQmlRequestor.h"

#include <QDebug>
#include <QQmlEngine>
#include <QUuid>
#include <QVariant>

#include <McIoc/ApplicationContext/IMcApplicationContext.h>

#include "McBoot/Connection/McQmlConnection.h"
#include "McBoot/Controller/IMcControllerContainer.h"
#include "McBoot/Controller/impl/McQmlResponse.h"
#include "McBoot/Requestor/McRequest.h"
#include "McBoot/Utils/Callback/Impl/McQmlSyncCallback.h"

MC_AUTO_INIT(McQmlRequestor)
MC_INIT_END

MC_DECL_PRIVATE_DATA(McQmlRequestor)
QMap<QString, McQmlConnection *> qmlConnections;
MC_DECL_PRIVATE_DATA_END

McQmlRequestor::McQmlRequestor(QObject *parent) : McAbstractRequestor(parent)
{
    MC_NEW_PRIVATE_DATA(McQmlRequestor);
}

McQmlRequestor::~McQmlRequestor()
{
    QMap<QString, McQmlConnection *> qmlConnections;
    qmlConnections.swap(d->qmlConnections);
    for (auto con : qmlConnections) {
        delete con;
    }
}

McQmlResponse *McQmlRequestor::invoke(const QString &uri,
                                      const QJSValue &data1,
                                      const QJSValue &data2) noexcept
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
    auto response = new McQmlResponse(); //!< 没有指定父对象，该对象将在整个请求完毕时被析构
    QQmlEngine::setObjectOwnership(response, QQmlEngine::CppOwnership);
    if (data1.isUndefined()) {
        run(response, uri, QVariant());
        return response;
    }
    if (data1.isCallable()) {
        QVariantMap m;
        m.insert(Mc::QuickBoot::Constant::Argument::qmlCallback,
                 QVariant::fromValue(McQmlSyncCallbackPtr(new McQmlSyncCallback(data1, engine),
                                                          IMcCallback::McCallbackDeleter())));
        run(response, uri, QVariant(m));
        return response;
    }
    auto m = data1.toVariant().toMap();
    if (!data2.isUndefined()) {
        m.insert(Mc::QuickBoot::Constant::Argument::qmlCallback,
                 QVariant::fromValue(McQmlSyncCallbackPtr(new McQmlSyncCallback(data2, engine),
                                                          IMcCallback::McCallbackDeleter())));
    }
    run(response, uri, QVariant(m));
    return response;
}

QJSValue McQmlRequestor::syncInvoke(const QString &uri) noexcept
{
    auto body = controllerContainer()->invoke(uri, QVariant(), McRequest());
#if QT_VERSION < QT_VERSION_CHECK(5, 5, 0)
    auto engine = QQmlEngine::contextForObject(this)->engine();
#else
    auto engine = qjsEngine(this);
#endif
    if (!engine) {
        return QJSValue();
    }
    auto arg = engine->toScriptValue(body);
    return arg;
}

QJSValue McQmlRequestor::syncInvoke(const QString &uri, const QJSValue &data) noexcept
{
    auto body = controllerContainer()->invoke(uri, data.toVariant(), McRequest());
#if QT_VERSION < QT_VERSION_CHECK(5, 5, 0)
    auto engine = QQmlEngine::contextForObject(this)->engine();
#else
    auto engine = qjsEngine(this);
#endif
    if (!engine) {
        return QJSValue();
    }
    auto arg = engine->toScriptValue(body);
    return arg;
}

QString McQmlRequestor::connect(const QString &beanName,
                                const QString &signal,
                                const QJSValue &callback) noexcept
{
    if (!callback.isCallable()) {
        qWarning() << "you have to pass in a callback function";
        return "";
    }
    auto bean = Mc::getObject(appCtx(), beanName);
    if (bean == nullptr) {
        return "";
    }
#if QT_VERSION < QT_VERSION_CHECK(5, 5, 0)
    auto engine = QQmlEngine::contextForObject(this)->engine();
#else
    auto engine = qjsEngine(this);
#endif
    if (engine == nullptr) {
        return "";
    }
    auto qmlConnection = new McQmlConnection();
    if (!qmlConnection->init(bean, signal, callback, engine)) {
        qmlConnection->deleteLater();
        return "";
    }
    QQmlEngine::setObjectOwnership(qmlConnection, QQmlEngine::CppOwnership);
    auto uuid = QUuid::createUuid().toString(QUuid::WithoutBraces);
    d->qmlConnections.insert(uuid, qmlConnection);
    connect(qmlConnection, &QObject::destroyed, this, [this, uuid](QObject *) {
        d->qmlConnections.remove(uuid);
    });
    return uuid;
}

void McQmlRequestor::disconnect(const QString &uuid) noexcept
{
    if (!d->qmlConnections.contains(uuid)) {
        return;
    }
    d->qmlConnections.value(uuid)->deleteLater();
}

void McQmlRequestor::disconnect(const QString &beanName,
                                const QString &signal,
                                const QJSValue &callback) noexcept
{
    auto bean = Mc::getObject(appCtx(), beanName);
    if (bean == nullptr) {
        return;
    }
    for (auto con : d->qmlConnections) {
        if (con->check(bean, signal, callback)) {
            con->deleteLater();
        }
    }
}
