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
#include "McPause.h"

MC_DECL_SHARED_PRIVATE_DATA2(McPause)
{
    QAtomicInteger<bool> isPaused{false};
    IMcCallbackPtr callback;
};

McPause::McPause() noexcept
{
    MC_NEW_SHARED_PRIVATE_DATA(McPause);
}

McPause::~McPause() {}

McPause::McPause(const McPause &o) noexcept
    : d(o.d)
{
}

McPause &McPause::operator=(const McPause &o) noexcept
{
    McPause copy(o);
    d.swap(copy.d);
    return *this;
}

McPause::McPause(McPause &&o) noexcept
    : d(o.d)
{
    o.d.reset();
}

McPause &McPause::operator=(McPause &&o) noexcept
{
    McPause copy(std::move(o));
    d.swap(copy.d);
    return *this;
}

bool McPause::isPaused() const noexcept
{
    return d->isPaused.loadAcquire();
}

void McPause::pause() noexcept
{
    setPaused(true);
}

void McPause::resume() noexcept
{
    setPaused(false);
}

void McPause::setPaused(bool val) noexcept
{
    d->isPaused.storeRelease(val);
    if (!d->callback.isNull()) {
        d->callback->call(d->isPaused.loadAcquire());
    }
}

QAtomicInteger<bool> &McPause::capture() noexcept
{
    return d->isPaused;
}

const QAtomicInteger<bool> &McPause::capture() const noexcept
{
    return d->isPaused;
}

void McPause::setCallback(const IMcCallbackPtr &val) noexcept
{
    d->callback = val;
}
