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
