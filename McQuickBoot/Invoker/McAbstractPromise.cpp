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
#include <QThread>

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
    QAtomicInteger<bool> isRunning{false};
    QAtomicInteger<bool> isFinished{false};
    QThread *targetThread{nullptr};
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

bool McAbstractPromise::isRunning() const noexcept
{
    return d->isRunning.loadRelaxed();
}

bool McAbstractPromise::isFinished() const noexcept
{
    return d->isFinished.loadRelaxed();
}

QThread *McAbstractPromise::targetThread() noexcept
{
    return d->targetThread;
}

QVariant McAbstractPromise::result() const noexcept
{
    return body();
}

bool McAbstractPromise::waitForStarted(qint64 msec) const noexcept
{
    return Mc::waitForExecFunc([this]() { return this->isStarted(); }, msec);
}

bool McAbstractPromise::waitForRunning(qint64 msec) const noexcept
{
    return Mc::waitForExecFunc([this]() { return this->isRunning(); }, msec);
}

bool McAbstractPromise::waitForFinished(qint64 msec) const noexcept
{
    return Mc::waitForExecFunc([this]() { return this->isFinished(); }, msec);
}

void McAbstractPromise::terminate(bool isWait, QDeadlineTimer deadline) const noexcept
{
    if (d->targetThread == nullptr) {
        return;
    }
    d->targetThread->terminate();
    if (isWait) {
        d->targetThread->wait(deadline);
    }
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

QVariant McAbstractPromise::body() const noexcept
{
    return d->body;
}

void McAbstractPromise::setBody(const QVariant &var) noexcept
{
    d->body = var;

    auto cleanup = qScopeGuard([this]() {
        auto func = [this]() { //!< 由于都是使用Qt::QueuedConnection标志，所以此函数一定后于callCallback等调用
            setFinished();
            deleteLater();
        };
        QMetaObject::invokeMethod(this, func, Qt::QueuedConnection);
    });

    //    for (const auto &handler : qAsConst(d->responseHanlders)) {
    //        if (!handler->handler(d->body)) {
    //            return;
    //        }
    //    }

    if (d->isAttached && d->attachedObject.isNull()) {
        return;
    }

    QObject *object = this;
    if (d->isAttached) {
        object = d->attachedObject.data();
    }

    if (isCanceled()) {
        QMetaObject::invokeMethod(
            object, [this]() { callCanceled(); }, Qt::QueuedConnection);
    } else if (d->body.canConvert<McResultPtr>() && d->body.value<McResultPtr>()->isInternalError()) {
        QMetaObject::invokeMethod(
            object, [this]() { callError(); }, Qt::QueuedConnection);
    } else if (isAsyncCall()) {
        callCallback();
    } else {
        QMetaObject::invokeMethod(
            object, [this]() { callCallback(); }, Qt::QueuedConnection);
    }
}

void McAbstractPromise::setStarted(bool val) noexcept
{
    d->isStarted.storeRelaxed(val);
}

void McAbstractPromise::setRunning(bool val) noexcept
{
    d->isRunning.storeRelaxed(val);
}

void McAbstractPromise::setFinished(bool val) noexcept
{
    d->isFinished.storeRelaxed(val);
}

void McAbstractPromise::setTargetThread(QThread *val) noexcept
{
    d->targetThread = val;
}
