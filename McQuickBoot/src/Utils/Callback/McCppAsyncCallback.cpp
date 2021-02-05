#include "McBoot/Utils/Callback/Impl/McCppAsyncCallback.h"

MC_STATIC()
qRegisterMetaType<McCppAsyncCallback>();
MC_REGISTER_BEAN_FACTORY(McCppAsyncCallback);
MC_STATIC_END

MC_DECL_PRIVATE_DATA(McCppAsyncCallback)
QList<bool> isQVariants;
const QObject *recever{nullptr};
QtPrivate::QSlotObjectBase *callback{nullptr};

void deinit()
{
    if (callback == nullptr) {
        return;
    }
    callback->destroyIfLastRef();
    callback = nullptr;
}
MC_PRIVATE_DATA_DESTRUCTOR(McCppAsyncCallback)
deinit();
MC_PRIVATE_DATA_DESTRUCTOR_END
MC_DECL_PRIVATE_DATA_END

McCppAsyncCallback::McCppAsyncCallback() noexcept
{
    MC_NEW_PRIVATE_DATA(McCppAsyncCallback);
}

McCppAsyncCallback::~McCppAsyncCallback() {}

McCppAsyncCallback::McCppAsyncCallback(const McCppAsyncCallback &o) noexcept : McCppAsyncCallback()
{
    d->isQVariants = o.d->isQVariants;
    d->recever = o.d->recever;
    d->callback = o.d->callback;
    d->callback->ref();
}

McCppAsyncCallback::McCppAsyncCallback(McCppAsyncCallback &&o) noexcept
{
    d.swap(o.d);
}

McCppAsyncCallback &McCppAsyncCallback::operator=(const McCppAsyncCallback &o) noexcept
{
    d->deinit();
    d->isQVariants = o.d->isQVariants;
    d->recever = o.d->recever;
    d->callback = o.d->callback;
    d->callback->ref();
    return *this;
}

McCppAsyncCallback &McCppAsyncCallback::operator=(McCppAsyncCallback &&o) noexcept
{
    d.swap(o.d);
    return *this;
}

void McCppAsyncCallback::call(const QVariantList &varList) noexcept
{
    Q_ASSERT_X(varList.length() >= d->isQVariants.length(),
               "McCppAsyncCallback::syncCall",
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

void McCppAsyncCallback::init(const QList<bool> &isQVariants,
                              const QObject *recever,
                              QtPrivate::QSlotObjectBase *callback) noexcept
{
    d->isQVariants = isQVariants;
    d->recever = recever;
    d->callback = callback;
}
