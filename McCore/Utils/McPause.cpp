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
#include "McPause.h"

MC_INIT(McPause)
mcRegisterMetaTypeSimple<McPause>();
MC_INIT_END

struct McPauseSharedData : public QSharedData
{
    QAtomicInteger<bool> isPaused{false};
    IMcCallbackPtr callback;
};

McPause::McPause() noexcept
{
    d = new McPauseSharedData();
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
