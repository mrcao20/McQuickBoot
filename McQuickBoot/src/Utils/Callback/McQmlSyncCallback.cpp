#include "McBoot/Utils/Callback/Impl/McQmlSyncCallback.h"

#include <QJSEngine>

#include "McBoot/Utils/McJsonUtils.h"

MC_STATIC()
qRegisterMetaType<McQmlSyncCallback>();
MC_REGISTER_BEAN_FACTORY(McQmlSyncCallback);
MC_STATIC_END

MC_DECL_PRIVATE_DATA(McQmlSyncCallback)
QJSValue callback;
QJSEngine *engine;
MC_DECL_PRIVATE_DATA_END

McQmlSyncCallback::McQmlSyncCallback()
{
    MC_NEW_PRIVATE_DATA(McQmlSyncCallback);
}

McQmlSyncCallback::McQmlSyncCallback(const QJSValue &callback, QJSEngine *engine) noexcept
    : McQmlSyncCallback()
{
    d->callback = callback;
    d->engine = engine;
}

McQmlSyncCallback::~McQmlSyncCallback() {}

McQmlSyncCallback::McQmlSyncCallback(const McQmlSyncCallback &o) noexcept : McQmlSyncCallback()
{
    operator=(o);
}

McQmlSyncCallback::McQmlSyncCallback(McQmlSyncCallback &&o) noexcept : McQmlSyncCallback()
{
    operator=(o);
}

void McQmlSyncCallback::operator=(const McQmlSyncCallback &o) noexcept
{
    d->callback = o.d->callback;
    d->engine = o.d->engine;
}

void McQmlSyncCallback::operator=(McQmlSyncCallback &&o) noexcept
{
    d->callback = o.d->callback;
    d->engine = o.d->engine;
}

void McQmlSyncCallback::syncCall(const QVariantList &varList) noexcept
{
    QJSValueList args;
    for (auto param : varList) {
        param = McJsonUtils::serialize(param);
        args << d->engine->toScriptValue(param);
    }
    d->callback.call(args);
}
