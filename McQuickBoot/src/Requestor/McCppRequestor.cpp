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
#include "McBoot/Requestor/McCppRequestor.h"

#include <QVariant>

#include <McIoc/ApplicationContext/IMcApplicationContext.h>

#include "McBoot/Connection/McCppConnection.h"
#include "McBoot/Controller/IMcControllerContainer.h"
#include "McBoot/Controller/impl/McCppResponse.h"
#include "McBoot/Requestor/McRequest.h"

MC_AUTO_INIT(McCppRequestor)
MC_INIT_END

MC_DECL_PRIVATE_DATA(McCppRequestor)
MC_DECL_PRIVATE_DATA_END

McCppRequestor::McCppRequestor(QObject *parent) : McAbstractRequestor(parent)
{
    MC_NEW_PRIVATE_DATA(McCppRequestor);
}

McCppRequestor::~McCppRequestor()
{
}

QMetaObject::Connection McCppRequestor::connect(const QString &sender,
                                                const QString &signal,
                                                const QString &receiver,
                                                const QString &slot,
                                                Qt::ConnectionType type) noexcept
{
    return Mc::Ioc::connect(appCtx(), sender, signal, receiver, slot, type);
}

QMetaObject::Connection McCppRequestor::connect(const QString &sender,
                                                const QString &signal,
                                                QObject *receiver,
                                                const QString &slot,
                                                Qt::ConnectionType type) noexcept
{
    return Mc::Ioc::connect(appCtx(), sender, signal, receiver, slot, type);
}

bool McCppRequestor::disconnect(const QString &sender,
                                const QString &signal,
                                const QString &receiver,
                                const QString &slot) noexcept
{
    return Mc::Ioc::disconnect(appCtx(), sender, signal, receiver, slot);
}

bool McCppRequestor::disconnect(const QString &sender,
                                const QString &signal,
                                QObject *receiver,
                                const QString &slot) noexcept
{
    return Mc::Ioc::disconnect(appCtx(), sender, signal, receiver, slot);
}

McCppResponse &McCppRequestor::invoke(const QString &uri) noexcept
{
    auto response = new McCppResponse();
    run(response, uri, QVariant());
    return *response; //!< 没有指定父对象，该对象将在整个请求完毕时被析构
}

McCppResponse &McCppRequestor::invoke(const QString &uri, const QJsonObject &data) noexcept
{
    auto response = new McCppResponse();
    run(response, uri, data);
    return *response; //!< 没有指定父对象，该对象将在整个请求完毕时被析构
}

McCppResponse &McCppRequestor::invoke(const QString &uri, const QVariant &data) noexcept
{
    return invoke(uri, QVariantList() << data);
}

McCppResponse &McCppRequestor::invoke(const QString &uri, const QVariantList &data) noexcept
{
    auto response = new McCppResponse();
    run(response, uri, data);
    return *response; //!< 没有指定父对象，该对象将在整个请求完毕时被析构
}

QVariant McCppRequestor::syncInvoke(const QString &uri) noexcept
{
    return controllerContainer()->invoke(uri, QVariant(), McRequest());
}

QVariant McCppRequestor::syncInvoke(const QString &uri, const QJsonObject &data) noexcept
{
    return controllerContainer()->invoke(uri, data, McRequest());
}

QVariant McCppRequestor::syncInvoke(const QString &uri, const QVariant &data) noexcept
{
    return syncInvoke(uri, QVariantList() << data);
}

QVariant McCppRequestor::syncInvoke(const QString &uri, const QVariantList &data) noexcept
{
    return controllerContainer()->invoke(uri, data, McRequest());
}

QMetaObject::Connection McCppRequestor::connectImpl(const QString &sender,
                                                    const QString &signal,
                                                    const QObject *receiver,
                                                    QtPrivate::QSlotObjectBase *slot,
                                                    Qt::ConnectionType type) noexcept
{
    auto senderObj = appCtx()->getBeanPointer<QObject>(sender);
    if (senderObj == nullptr) {
        senderObj = appCtx()->getBean<QObject>(sender).data();
    }
    if (senderObj == nullptr) {
        qCritical("not exists bean '%s'", qPrintable(sender));
        return QMetaObject::Connection();
    }
    McCppConnection *con = new McCppConnection();
    auto c = con->init(senderObj, signal, const_cast<QObject *>(receiver), slot, type);
    if (!c) {
        con->deleteLater();
    }
    return c;
}
