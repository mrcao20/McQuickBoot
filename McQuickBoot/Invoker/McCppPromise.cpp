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
#include "McCppPromise.h"

MC_DECL_PRIVATE_DATA2(McCppPromise)
{
    McSlotObjectWrapper callback;
    McSlotObjectWrapper canceled;
    McSlotObjectWrapper error;
};

McCppPromise::McCppPromise(QObject *parent) noexcept
    : McAbstractPromise(parent)
{
    MC_NEW_PRIVATE_DATA(McCppPromise);
}

McCppPromise::~McCppPromise() {}

QPointer<McCppPromise> McCppPromise::capture() noexcept
{
    QPointer<McCppPromise> p(this);
    return p;
}

void McCppPromise::callCallback() noexcept
{
    d->callback.call(body());
}

void McCppPromise::callCanceled() noexcept
{
    d->canceled.call(body());
}

void McCppPromise::callError() noexcept
{
    d->error.call(body());
}

void McCppPromise::setCallback(CallbackType type, const McSlotObjectWrapper &functor) noexcept
{
    switch (type) {
    case CallbackType::Sync:
        d->callback = functor;
        setAsyncCall(false);
        break;
    case CallbackType::Async:
        d->callback = functor;
        setAsyncCall(true);
        break;
    case CallbackType::Canceled:
        d->canceled = functor;
        break;
    case CallbackType::Error:
        d->error = functor;
        break;
    default:
        break;
    }
}
