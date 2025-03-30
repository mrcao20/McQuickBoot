/*
 * Copyright (c) 2021 mrcao20/mrcao20@163.com
 * McQuickBoot is licensed under Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at:
 *          http://license.coscl.org.cn/MulanPSL2
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
 * EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
 * MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
 * See the Mulan PSL v2 for more details.
 */
#include "McQmlPromise.h"

#include <QJSEngine>
#include <QJSValue>

#include <McCore/Utils/McJsonUtils.h>

MC_STATIC2()
{
    qRegisterMetaType<McQmlPromise *>("McQmlPromise");
}

MC_DECL_PRIVATE_DATA2(McQmlPromise)
{
    QJSValue callback;
    QJSValue canceled;
    QJSValue error;
};

McQmlPromise::McQmlPromise(QObject *parent) noexcept
    : McAbstractPromise(parent)
{
    MC_NEW_PRIVATE_DATA(McQmlPromise);
}

McQmlPromise::~McQmlPromise() {}

McQmlPromise *McQmlPromise::then(const QJSValue &callback) noexcept
{
    d->callback = callback;
    setAsyncCall(false);
    return this;
}

McQmlPromise *McQmlPromise::syncThen(const QJSValue &callback) noexcept
{
    d->callback = callback;
    setAsyncCall(false);
    return this;
}

McQmlPromise *McQmlPromise::asyncThen(const QJSValue &callback) noexcept
{
    d->callback = callback;
    setAsyncCall(true);
    return this;
}

McQmlPromise *McQmlPromise::onCanceled(const QJSValue &callback) noexcept
{
    d->canceled = callback;
    return this;
}

McQmlPromise *McQmlPromise::onError(const QJSValue &callback) noexcept
{
    d->error = callback;
    return this;
}

McQmlPromise *McQmlPromise::onCurrentProgress(const QJSValue &callback) noexcept
{
    auto tmp = callback;
    getProgress().onCurrentCallback([tmp](int current) mutable { tmp.call(QJSValueList() << QJSValue(current)); });
    return this;
}

McQmlPromise *McQmlPromise::onTotalProgress(const QJSValue &callback) noexcept
{
    auto tmp = callback;
    getProgress().onTotalCallback([tmp](int total) mutable { tmp.call(QJSValueList() << QJSValue(total)); });
    return this;
}

void McQmlPromise::callCallback() noexcept
{
    call(d->callback);
}

void McQmlPromise::callCanceled() noexcept
{
    call(d->canceled);
}

void McQmlPromise::callError() noexcept
{
    call(d->error);
}

void McQmlPromise::call(QJSValue &callback) noexcept
{
    if (!callback.isCallable()) {
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
    callback.call(QJSValueList() << arg);
}
