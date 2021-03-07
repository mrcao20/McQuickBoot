#include "McBoot/Connection/McQmlConnection.h"

#include <QDebug>
#include <QJSEngine>
#include <QMetaMethod>

#include "McBoot/Utils/McJsonUtils.h"

MC_DECL_PRIVATE_DATA(McQmlConnection)
QObject *bean{nullptr};
QMetaMethod sig;
QJSValue callback;
QJSEngine *jsEngine;
MC_DECL_PRIVATE_DATA_END

McQmlConnection::McQmlConnection(QObject *parent) noexcept : McAbstractConnection(parent)
{
    MC_NEW_PRIVATE_DATA(McQmlConnection);
}

McQmlConnection::~McQmlConnection()
{
}

bool McQmlConnection::init(QObject *obj,
                           const QString &signal,
                           const QJSValue &callback,
                           QJSEngine *jsEngine) noexcept
{
    McAbstractConnection::init(obj, signal, Qt::QueuedConnection, d->sig);
    if (!d->sig.isValid()) {
        return false;
    }
    setParent(obj);
    d->bean = obj;
    d->callback = callback;
    d->jsEngine = jsEngine;
    return true;
}

bool McQmlConnection::check(QObject *obj, const QString &signal, const QJSValue &callback) noexcept
{
    if (d->bean != obj) {
        return false;
    }
    if (!callback.isUndefined() && !d->callback.strictlyEquals(callback)) {
        return false;
    }
    if (signal.isEmpty()) {
        return true;
    }
    if (signal.indexOf('(') == -1) {
        return d->sig.name() == signal;
    } else {
        return d->sig.methodSignature() == signal;
    }
}

void McQmlConnection::call(void **arguments) noexcept
{
    QJSValueList args;
    for (int i = 0; i < d->sig.parameterCount(); ++i) {
        QVariant param(d->sig.parameterType(i), arguments[i + 1]);
        param = McJsonUtils::serialize(param);
        args << d->jsEngine->toScriptValue(param);
    }
    d->callback.call(args);
}
