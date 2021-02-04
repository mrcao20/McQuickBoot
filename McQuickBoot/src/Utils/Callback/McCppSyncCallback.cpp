#include "McBoot/Utils/Callback/Impl/McCppSyncCallback.h"

MC_STATIC()
qRegisterMetaType<McCppSyncCallback>();
MC_REGISTER_BEAN_FACTORY(McCppSyncCallback);
MC_STATIC_END

MC_DECL_PRIVATE_DATA(McCppSyncCallback)
QList<bool> isQVariants;
const QObject *recever{nullptr};
QtPrivate::QSlotObjectBase *callback{nullptr};
MC_DECL_PRIVATE_DATA_END

McCppSyncCallback::McCppSyncCallback() noexcept
{
    MC_NEW_PRIVATE_DATA(McCppSyncCallback);
}

McCppSyncCallback::~McCppSyncCallback()
{
    if (d->callback == nullptr) {
        return;
    }
    d->callback->destroyIfLastRef();
    d->callback = nullptr;
}

McCppSyncCallback::McCppSyncCallback(const McCppSyncCallback &o) noexcept : McCppSyncCallback()
{
    operator=(o);
}

McCppSyncCallback::McCppSyncCallback(McCppSyncCallback &&o) noexcept : McCppSyncCallback()
{
    operator=(o);
}

void McCppSyncCallback::operator=(const McCppSyncCallback &o) noexcept
{
    d->recever = o.d->recever;
    d->callback = o.d->callback;
    d->callback->ref();
}

void McCppSyncCallback::operator=(McCppSyncCallback &&o) noexcept
{
    d->recever = o.d->recever;
    d->callback = o.d->callback;
    o.d->recever = nullptr;
    o.d->callback = nullptr;
}

void McCppSyncCallback::syncCall(const QVariantList &varList) noexcept
{
    Q_ASSERT_X(varList.length() >= d->isQVariants.length(),
               "McCppSyncCallback::syncCall",
               "recever argument count must be equal to or less than sender");
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
    d->callback->call(const_cast<QObject *>(d->recever), args);
    delete[] args;
}

void McCppSyncCallback::init(const QList<bool> &isQVariants,
                             const QObject *recever,
                             QtPrivate::QSlotObjectBase *callback) noexcept
{
    d->isQVariants = isQVariants;
    d->recever = recever;
    d->callback = callback;
}
