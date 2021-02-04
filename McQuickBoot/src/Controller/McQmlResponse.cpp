#include "McBoot/Controller/impl/McQmlResponse.h"

#include <QJSEngine>
#include <QJSValue>
#include <QJsonObject>
#include <QMetaClassInfo>
#include <QVariant>

#include <McIoc/BeanFactory/impl/McMetaTypeId.h>
#include <McIoc/Utils/McScopedFunction.h>

#include "McBoot/Utils/McJsonUtils.h"

MC_DECL_PRIVATE_DATA(McQmlResponse)
QJSValue callback;
MC_DECL_PRIVATE_DATA_END

MC_INIT(McQmlResponse)
qRegisterMetaType<McQmlResponse *>("McQmlResponse");
MC_INIT_END

McQmlResponse::McQmlResponse(QObject *parent) : McAbstractResponse(parent)
{
    MC_NEW_PRIVATE_DATA(McQmlResponse);
}

McQmlResponse::~McQmlResponse()
{
}

McQmlResponse *McQmlResponse::then(const QJSValue &callback) noexcept 
{
    d->callback = callback;
    setAsyncCall(false);
    return this;
}

McQmlResponse *McQmlResponse::syncThen(const QJSValue &callback) noexcept
{
    d->callback = callback;
    setAsyncCall(false);
    return this;
}

McQmlResponse *McQmlResponse::asyncThen(const QJSValue &callback) noexcept
{
    d->callback = callback;
    setAsyncCall(true);
    return this;
}

void McQmlResponse::callCallback() noexcept 
{
    McScopedFunction func([this](){
        this->deleteLater();
    });
    Q_UNUSED(func)
    
    if(!d->callback.isCallable()) {
        return;
    }
#if QT_VERSION < QT_VERSION_CHECK(5, 5, 0)
    auto engine = QQmlEngine::contextForObject(this)->engine();
#else
    auto engine = qjsEngine(this);
#endif
    if(!engine) {
        return;
    }
    auto body = McJsonUtils::serialize(this->body());
    auto arg = engine->toScriptValue(body);
    d->callback.call(QJSValueList() << arg);
}
