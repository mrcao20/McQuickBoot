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

McQmlConnection::McQmlConnection(QObject *parent) noexcept : QObject(parent)
{
    MC_NEW_PRIVATE_DATA(McQmlConnection);
}

McQmlConnection::~McQmlConnection()
{
    if (d->bean == nullptr) {
        return;
    }
    QMetaObject::disconnect(d->bean, d->sig.methodIndex(), this, metaObject()->methodCount());
}

bool McQmlConnection::init(QObject *obj,
                           const QString &signal,
                           const QJSValue &callback,
                           QJSEngine *jsEngine) noexcept
{
    auto senderMetaObj = obj->metaObject();
    if (signal.indexOf('(') == -1) {
        for (int i = 0; i < senderMetaObj->methodCount(); ++i) {
            auto m = senderMetaObj->method(i);
            if (m.methodType() != QMetaMethod::Signal) {
                continue;
            }
            if (m.name() != signal) {
                continue;
            }
            d->sig = m;
            break;
        }
    } else {
        auto index = senderMetaObj->indexOfSignal(signal.toLocal8Bit());
        d->sig = senderMetaObj->method(index);
    }
    if (!d->sig.isValid()) {
        qWarning() << "cannot found signal:" << signal
                   << "from class:" << senderMetaObj->className();
        return false;
    }
    setParent(obj);
    d->bean = obj;
    d->callback = callback;
    d->jsEngine = jsEngine;

    QMetaObject::connect(obj,
                         d->sig.methodIndex(),
                         this,
                         metaObject()->methodCount(),
                         Qt::QueuedConnection);
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

int McQmlConnection::qt_metacall(QMetaObject::Call c, int id, void **arguments)
{
    id = QObject::qt_metacall(c, id, arguments);
    if (id < 0 || c != QMetaObject::InvokeMetaMethod)
        return id;
    QJSValueList args;
    for (int i = 0; i < d->sig.parameterCount(); ++i) {
        QVariant param(d->sig.parameterType(i), arguments[i + 1]);
        param = McJsonUtils::serialize(param);
        args << d->jsEngine->toScriptValue(param);
    }
    d->callback.call(args);
    return -1;
}
