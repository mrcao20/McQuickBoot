#include "McBoot/Requestor/McQmlRequestor.h"

#include <QQmlEngine>
#include <QVariant>

#include "McBoot/Controller/impl/McQmlResponse.h"
#include "McBoot/Socket/IMcQmlSocketContainer.h"
#include "McBoot/Socket/impl/McQmlSocket.h"

MC_AUTO_INIT(McQmlRequestor)
MC_INIT_END

MC_DECL_PRIVATE_DATA(McQmlRequestor)
IMcQmlSocketContainerPtr qmlSocketContainer;
MC_DECL_PRIVATE_DATA_END

McQmlRequestor::McQmlRequestor(QObject *parent) : McAbstractRequestor(parent)
{
    MC_NEW_PRIVATE_DATA(McQmlRequestor);
}

McQmlRequestor::~McQmlRequestor() 
{
    qDebug() << "~McQmlRequestor";
}

void McQmlRequestor::setSocketContainer(IMcQmlSocketContainerConstPtrRef val) noexcept
{
    d->qmlSocketContainer = val;
}

McQmlResponse *McQmlRequestor::invoke(const QString &uri) noexcept
{
    auto response = new McQmlResponse();
    QQmlEngine::setObjectOwnership(response, QQmlEngine::CppOwnership);
    run(response, uri, QVariant());
    return response; //!< 没有指定父对象，该对象将在整个请求完毕时被析构
}

McQmlResponse *McQmlRequestor::invoke(const QString &uri, const QJsonObject &data) noexcept
{
    auto response = new McQmlResponse();
    QQmlEngine::setObjectOwnership(response, QQmlEngine::CppOwnership);
    run(response, uri, data);
    return response; //!< 没有指定父对象，该对象将在整个请求完毕时被析构
}

McQmlSocket *McQmlRequestor::addConnect(const QString &uri) noexcept
{
    auto qs = d->qmlSocketContainer->addConnect(uri);
    QQmlEngine::setObjectOwnership(qs, QQmlEngine::JavaScriptOwnership);
    return qs;
}

McQmlSocket *McQmlRequestor::addConnect(const QString &uri, const QJSValue &data) noexcept
{
    auto qs = d->qmlSocketContainer->addConnect(uri, data);
    QQmlEngine::setObjectOwnership(qs, QQmlEngine::JavaScriptOwnership);
    return qs;
}
