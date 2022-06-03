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
#include "McCancel.h"

MC_INIT(McCancel)
mcRegisterMetaTypeSimple<McCancel>();
MC_INIT_END

struct McCancelSharedData : public QSharedData
{
    QAtomicInteger<bool> isCanceled{false};
    IMcCallbackPtr callback;
};

McCancel::McCancel() noexcept
{
    d = new McCancelSharedData();
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
