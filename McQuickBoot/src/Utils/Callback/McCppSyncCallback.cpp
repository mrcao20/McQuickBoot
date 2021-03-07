#include "McBoot/Utils/Callback/Impl/McCppSyncCallback.h"

MC_STATIC()
qRegisterMetaType<McCppSyncCallback>();
MC_REGISTER_BEAN_FACTORY(McCppSyncCallback);
MC_STATIC_END

MC_DECL_PRIVATE_DATA(McCppSyncCallback)
QList<bool> isQVariants;
const QObject *receiver{nullptr};
QtPrivate::QSlotObjectBase *callback{nullptr};

void deinit()
{
    if (callback == nullptr) {
        return;
    }
    callback->destroyIfLastRef();
    callback = nullptr;
}
MC_PRIVATE_DATA_DESTRUCTOR(McCppSyncCallback)
deinit();
MC_PRIVATE_DATA_DESTRUCTOR_END
MC_DECL_PRIVATE_DATA_END

McCppSyncCallback::McCppSyncCallback() noexcept
{
    MC_NEW_PRIVATE_DATA(McCppSyncCallback);
}

McCppSyncCallback::~McCppSyncCallback() {}

McCppSyncCallback::McCppSyncCallback(const McCppSyncCallback &o) noexcept : McCppSyncCallback()
{
    d->isQVariants = o.d->isQVariants;
    d->receiver = o.d->receiver;
    d->callback = o.d->callback;
    d->callback->ref();
}

McCppSyncCallback::McCppSyncCallback(McCppSyncCallback &&o) noexcept
{
    d.swap(o.d);
}

McCppSyncCallback &McCppSyncCallback::operator=(const McCppSyncCallback &o) noexcept
{
    d->deinit();
    d->isQVariants = o.d->isQVariants;
    d->receiver = o.d->receiver;
    d->callback = o.d->callback;
    d->callback->ref();
    return *this;
}

McCppSyncCallback &McCppSyncCallback::operator=(McCppSyncCallback &&o) noexcept
{
    d.swap(o.d);
    return *this;
}

void McCppSyncCallback::syncCall(const QVariantList &varList) noexcept
{
    Q_ASSERT_X(varList.length() >= d->isQVariants.length(),
               "McCppSyncCallback::syncCall",
               "receiver argument count must be equal to or less than sender");
    void **args = new void *[varList.length() + 1];
    args[0] = nullptr;
    for (int i = 0; i < varList.length(); ++i) {
        decltype(auto) var = varList.at(i);
        if (i < d->isQVariants.length() && d->isQVariants.at(i)) {
            args[i + 1] = const_cast<void *>((const void *) &var);
        } else {
            args[i + 1] = const_cast<void *>(var.data());
        }
    }
    d->callback->call(const_cast<QObject *>(d->receiver), args);
    delete[] args;
}

void McCppSyncCallback::init(const QList<bool> &isQVariants,
                             const QObject *receiver,
                             QtPrivate::QSlotObjectBase *callback) noexcept
{
    d->isQVariants = isQVariants;
    d->receiver = receiver;
    d->callback = callback;
}
