#include "McBoot/Controller/impl/McQmlResponse.h"

#include <QJSEngine>
#include <QJSValue>
#include <QJsonObject>
#include <QMetaClassInfo>
#include <QVariant>

#include <McIoc/BeanFactory/impl/McMetaTypeId.h>

#include "McBoot/Utils/McJsonUtils.h"

MC_DECL_PRIVATE_DATA(McQmlResponse)
QJSValue callback;
QJSValue error;
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

McQmlResponse *McQmlResponse::error(const QJSValue &func) noexcept
{
    d->error = func;
    setAsyncCall(false);
    return this;
}

McQmlResponse *McQmlResponse::progress(const QJSValue &callback) noexcept
{
    auto tmp = callback;
    getProgress().callback([tmp](int current, int total) mutable {
        tmp.call(QJSValueList() << QJSValue(current) << QJSValue(total));
    });
    return this;
}

void McQmlResponse::callCallback() noexcept 
{
    call(d->callback);
}

void McQmlResponse::callError() noexcept
{
    call(d->error);
}

void McQmlResponse::call(QJSValue &func) noexcept
{
    if (!func.isCallable()) {
        return;
    }
#if QT_VERSION < QT_VERSION_CHECK(5, 5, 0)
    auto engine = QQmlEngine::contextForObject(this)->engine();
#else
    auto engine = qjsEngine(this);
#endif
    if (!engine) {
        return;
    }
    auto body = McJsonUtils::serialize(this->body());
    auto arg = engine->toScriptValue(body);
    func.call(QJSValueList() << arg);
}
