#include "McBoot/Requestor/McQmlRequestor.h"

#include <QQmlEngine>
#include <QVariant>

#include "McBoot/Controller/IMcControllerContainer.h"
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

McQmlRequestor::~McQmlRequestor() {}

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

McQmlResponse *McQmlRequestor::invoke(const QString &uri, const QJSValue &data) noexcept
{
    auto response = new McQmlResponse();
    QQmlEngine::setObjectOwnership(response, QQmlEngine::CppOwnership);
    run(response, uri, data.toVariant());
    return response; //!< 没有指定父对象，该对象将在整个请求完毕时被析构
}

QJSValue McQmlRequestor::syncInvoke(const QString &uri) noexcept
{
    auto body = controllerContainer()->invoke(uri, QVariant());
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
    auto body = controllerContainer()->invoke(uri, data.toVariant());
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
