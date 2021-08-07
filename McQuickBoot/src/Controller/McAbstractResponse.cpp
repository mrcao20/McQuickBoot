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
#include "McBoot/Controller/impl/McAbstractResponse.h"

#include <QCoreApplication>
#include <QEvent>
#include <QPointer>
#include <QVariant>

#include <McIoc/Utils/McScopedFunction.h>

#include "McBoot/Controller/impl/McResult.h"
#include "McBoot/Utils/Response/IMcResponseHandler.h"

MC_DECL_PRIVATE_DATA(McAbstractResponse)
bool isAsyncCall{false}; //! 是否在次线程调用callback，默认不需要
McCancel cancel;
McProgress progress;
QVariant body;
QPointer<QObject> attachedObject;
bool isAttached{false};
QList<IMcResponseHandlerPtr> responseHanlders;
MC_DECL_PRIVATE_DATA_END

MC_INIT(McAbstractResponse)
MC_INIT_END

McAbstractResponse::McAbstractResponse(QObject *parent) : QObject(parent)
{
    MC_NEW_PRIVATE_DATA(McAbstractResponse);
}

McAbstractResponse::~McAbstractResponse() {}

void McAbstractResponse::setHandlers(const QList<IMcResponseHandlerPtr> &val) noexcept
{
    d->responseHanlders = val;
}

void McAbstractResponse::cancel() noexcept
{
    d->cancel.cancel();
}

bool McAbstractResponse::isCanceled() const noexcept
{
    return d->cancel.isCanceled();
}

QVariant McAbstractResponse::body() const noexcept
{
    return d->body;
}

void McAbstractResponse::setBody(const QVariant &var) noexcept
{
    d->body = var;

    if (isAsyncCall()) {
        call();
        return;
    }

    //! 发布的事件由QT删除
    qApp->postEvent(this, new QEvent(static_cast<QEvent::Type>(QEvent::Type::User + 1)));
}

bool McAbstractResponse::isAsyncCall() const noexcept
{
    return d->isAsyncCall;
}

void McAbstractResponse::setAsyncCall(bool val) noexcept
{
    d->isAsyncCall = val;
}

void McAbstractResponse::attach(QObject *obj) noexcept
{
    d->isAttached = true;
    d->attachedObject = obj;
}

void McAbstractResponse::detach() noexcept
{
    d->isAttached = false;
    d->attachedObject = nullptr;
}

void McAbstractResponse::customEvent(QEvent *event)
{
    if (event->type() == QEvent::Type::User + 1) {
        call();
    }
}

McProgress &McAbstractResponse::getProgress() const noexcept
{
    return d->progress;
}

McCancel &McAbstractResponse::getCancel() const noexcept
{
    return d->cancel;
}

void McAbstractResponse::call() noexcept
{
    McScopedFunction cleanup([this]() { this->deleteLater(); });
    Q_UNUSED(cleanup)

    for (const auto &handler : qAsConst(d->responseHanlders)) {
        if (!handler->handler(d->body)) {
            return;
        }
    }

    if (d->isAttached && d->attachedObject.isNull()) {
        return;
    }

    if (!d->body.canConvert<McResultPtr>()) {
        callCallback();
        return;
    }
    auto result = d->body.value<McResultPtr>();
    if (result->isInternalError()) {
        qCWarning(mcQuickBoot()) << result;
        callError();
    } else {
        callCallback();
    }
}
