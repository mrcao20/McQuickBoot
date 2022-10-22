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
#include "McAbstractPromise.h"

#include <QCoreApplication>
#include <QScopeGuard>

#include "McResult.h"

MC_DECL_PRIVATE_DATA2(McAbstractPromise)
{
    bool isAsyncCall{false}; //! 是否在次线程调用callback，默认不需要
    McCancel cancel;
    McPause pause;
    McProgress progress;
    QVariant body;
    QPointer<QObject> attachedObject;
    bool isAttached{false};
    QAtomicInteger<bool> isStarted{false};
    QAtomicInteger<bool> isFinished{false};
};

McAbstractPromise::McAbstractPromise(QObject *parent) noexcept
    : QObject(parent)
{
    MC_NEW_PRIVATE_DATA(McAbstractPromise);
}

McAbstractPromise::~McAbstractPromise() {}

void McAbstractPromise::cancel() noexcept
{
    d->cancel.cancel();
}

bool McAbstractPromise::isCanceled() const noexcept
{
    return d->cancel.isCanceled();
}

void McAbstractPromise::pause() noexcept
{
    d->pause.pause();
}

void McAbstractPromise::resume() noexcept
{
    d->pause.resume();
}

bool McAbstractPromise::isPaused() const noexcept
{
    return d->pause.isPaused();
}

bool McAbstractPromise::isStarted() const noexcept
{
    return d->isStarted.loadRelaxed();
}

bool McAbstractPromise::isFinished() const noexcept
{
    return d->isFinished.loadRelaxed();
}

QVariant McAbstractPromise::result() const noexcept
{
    return body();
}

bool McAbstractPromise::waitForFinished(qint64 msec) const noexcept
{
    return Mc::waitForExecFunc([this]() { return this->isFinished(); }, msec);
}

bool McAbstractPromise::isAsyncCall() const noexcept
{
    return d->isAsyncCall;
}

void McAbstractPromise::setAsyncCall(bool val) noexcept
{
    d->isAsyncCall = val;
}

void McAbstractPromise::attach(QObject *obj) noexcept
{
    d->isAttached = true;
    d->attachedObject = obj;
}

void McAbstractPromise::detach() noexcept
{
    d->isAttached = false;
    d->attachedObject = nullptr;
}

void McAbstractPromise::customEvent(QEvent *event)
{
    if (event->type() == QEvent::Type::User) {
        call();
    }
}

QVariant McAbstractPromise::body() const noexcept
{
    return d->body;
}

void McAbstractPromise::setBody(const QVariant &var) noexcept
{
    d->body = var;

    if (isAsyncCall()) {
        call();
        return;
    }

    //! 发布的事件由QT删除
    qApp->postEvent(this, new QEvent(static_cast<QEvent::Type>(QEvent::Type::User)));
}

void McAbstractPromise::setStarted(bool val) noexcept
{
    d->isStarted.storeRelaxed(val);
}

void McAbstractPromise::setFinished(bool val) noexcept
{
    d->isFinished.storeRelaxed(val);
}

McProgress &McAbstractPromise::getProgress() const noexcept
{
    return d->progress;
}

McCancel &McAbstractPromise::getCancel() const noexcept
{
    return d->cancel;
}

McPause &McAbstractPromise::getPause() const noexcept
{
    return d->pause;
}

void McAbstractPromise::call() noexcept
{
    auto cleanup = qScopeGuard([this]() {
        this->setFinished();
        this->deleteLater();
    });

    //    for (const auto &handler : qAsConst(d->responseHanlders)) {
    //        if (!handler->handler(d->body)) {
    //            return;
    //        }
    //    }

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
