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
#include "McProgress.h"

MC_INIT(McProgress)
mcRegisterMetaTypeSimple<McProgress>();
MC_INIT_END

struct McProgressSharedData : public QSharedData
{
    QAtomicInt current{0};
    QAtomicInt total{100};
    IMcCallbackPtr currentCallback;
    IMcCallbackPtr totalCallback;
};

McProgress::McProgress() noexcept
{
    d = new McProgressSharedData();
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
