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
#include "McProgress.h"

MC_DECL_SHARED_PRIVATE_DATA2(McProgress)
{
    QAtomicInt current{0};
    QAtomicInt total{100};
    IMcCallbackPtr currentCallback;
    IMcCallbackPtr totalCallback;
};

McProgress::McProgress() noexcept
{
    MC_NEW_SHARED_PRIVATE_DATA(McProgress);
}

McProgress::~McProgress() {}

McProgress::McProgress(const McProgress &o) noexcept
    : d(o.d)
{
}

McProgress &McProgress::operator=(const McProgress &o) noexcept
{
    McProgress copy(o);
    d.swap(copy.d);
    return *this;
}

McProgress::McProgress(McProgress &&o) noexcept
    : d(o.d)
{
    o.d.reset();
}

McProgress &McProgress::operator=(McProgress &&o) noexcept
{
    McProgress copy(std::move(o));
    d.swap(copy.d);
    return *this;
}

int McProgress::current() const noexcept
{
    return d->current.loadAcquire();
}

void McProgress::setCurrent(int val) noexcept
{
    d->current.storeRelease(val);
    if (!d->currentCallback.isNull()) {
        d->currentCallback->call(d->current.loadAcquire());
    }
}

void McProgress::increaseCurrent(int val) noexcept
{
    d->current.fetchAndAddRelease(val);
    if (!d->currentCallback.isNull()) {
        d->currentCallback->call(d->current.loadAcquire());
    }
}

int McProgress::total() const noexcept
{
    return d->total.loadAcquire();
}

void McProgress::setTotal(int val) noexcept
{
    d->total.storeRelease(val);
    if (!d->totalCallback.isNull()) {
        d->totalCallback->call(d->total.loadAcquire());
    }
}

void McProgress::setCurrentCallback(const IMcCallbackPtr &val) noexcept
{
    d->currentCallback = val;
}

void McProgress::setTotalCallback(const IMcCallbackPtr &val) noexcept
{
    d->totalCallback = val;
}
