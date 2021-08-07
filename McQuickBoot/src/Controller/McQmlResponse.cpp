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
