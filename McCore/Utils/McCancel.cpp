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
#include "McCancel.h"

MC_DECL_SHARED_PRIVATE_DATA2(McCancel)
{
    QAtomicInteger<bool> isCanceled{false};
    IMcCallbackPtr callback;
};

McCancel::McCancel() noexcept
{
    MC_NEW_SHARED_PRIVATE_DATA(McCancel);
}

McCancel::~McCancel() {}

McCancel::McCancel(const McCancel &o) noexcept
    : d(o.d)
{
}

McCancel &McCancel::operator=(const McCancel &o) noexcept
{
    McCancel copy(o);
    d.swap(copy.d);
    return *this;
}

McCancel::McCancel(McCancel &&o) noexcept
    : d(o.d)
{
    o.d.reset();
}

McCancel &McCancel::operator=(McCancel &&o) noexcept
{
    McCancel copy(std::move(o));
    d.swap(copy.d);
    return *this;
}

bool McCancel::isCanceled() const noexcept
{
    return d->isCanceled.loadAcquire();
}

void McCancel::cancel() noexcept
{
    d->isCanceled.storeRelease(true);
    if (!d->callback.isNull()) {
        d->callback->call(d->isCanceled.loadAcquire());
    }
}

QAtomicInteger<bool> &McCancel::capture() noexcept
{
    return d->isCanceled;
}

const QAtomicInteger<bool> &McCancel::capture() const noexcept
{
    return d->isCanceled;
}

void McCancel::setCallback(const IMcCallbackPtr &val) noexcept
{
    d->callback = val;
}
