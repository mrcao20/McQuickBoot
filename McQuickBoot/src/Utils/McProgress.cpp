#include "McBoot/Utils/McProgress.h"

MC_INIT(McProgress)
qRegisterMetaType<McProgress>();
MC_INIT_END

McProgress::McProgress() noexcept
{
    d = new McProgressSharedData();
    d->attached.reset(new QObject());
}

McProgress::~McProgress() {}

McProgress::McProgress(const McProgress &o) noexcept : d(o.d) {}

McProgress::McProgress(McProgress &&o) noexcept
{
    d.swap(o.d);
}

McProgress &McProgress::operator=(const McProgress &o) noexcept
{
    d = o.d;
    return *this;
}

McProgress &McProgress::operator=(McProgress &&o) noexcept
{
    d.swap(o.d);
    return *this;
}

int McProgress::current() const noexcept
{
    return d->current.loadAcquire();
}

void McProgress::setCurrent(int val) noexcept
{
    d->current.storeRelease(val);
    callCallback();
}

int McProgress::total() const noexcept
{
    return d->total.loadAcquire();
}

void McProgress::setTotal(int val) noexcept
{
    d->total.storeRelease(val);
    callCallback();
}

void McProgress::setCallback(const QObject *recever, QtPrivate::QSlotObjectBase *callback) noexcept
{
    d->recever.storeRelease(const_cast<QObject *>(recever));
    d->callback.storeRelease(callback);
}

void McProgress::callCallback() noexcept
{
    auto dd = d;
    QMetaObject::invokeMethod(
        d->attached.data(),
        [dd]() {
            auto callback = dd->callback.loadAcquire();
            if (callback != nullptr) {
                auto curr = dd->current.loadAcquire();
                auto tot = dd->total.loadAcquire();
                void *args[] = {nullptr, &curr, &tot};
                callback->call(dd->recever.loadAcquire(), args);
            }
        },
        Qt::QueuedConnection);
}
