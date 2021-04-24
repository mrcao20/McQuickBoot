#include "McBoot/Controller/impl/McCppResponse.h"

#include <QQmlEngine>
#include <QVariant>

#include <McIoc/Utils/McScopedFunction.h>

#include "McBoot/Controller/impl/McResult.h"

MC_DECL_PRIVATE_DATA(McCppResponse)
bool isQVariant{false};
const QObject *recever{nullptr};
QtPrivate::QSlotObjectBase *callback{nullptr};
QtPrivate::QSlotObjectBase *error{nullptr};
MC_DECL_PRIVATE_DATA_END

MC_INIT(McCppResponse)
MC_INIT_END

McCppResponse::McCppResponse(QObject *parent) : McAbstractResponse(parent)
{
    MC_NEW_PRIVATE_DATA(McCppResponse);
}

McCppResponse::~McCppResponse()
{
    if (d->callback == nullptr) {
        return;
    }
    d->callback->destroyIfLastRef();
    d->callback = nullptr;
}

void McCppResponse::callCallback() noexcept
{
    call(d->callback);
}

void McCppResponse::callError() noexcept
{
    call(d->error);
}

McCppResponse &McCppResponse::thenImpl(bool isQVariant,
                                       const QObject *recever,
                                       QtPrivate::QSlotObjectBase *callback) noexcept
{
    d->isQVariant = isQVariant;
    d->recever = recever;
    d->callback = callback;
    setAsyncCall(false);
    return *this;
}

McCppResponse &McCppResponse::syncThenImpl(bool isQVariant,
                                           const QObject *recever,
                                           QtPrivate::QSlotObjectBase *callback) noexcept
{
    return thenImpl(isQVariant, recever, callback);
}

McCppResponse &McCppResponse::asyncThenImpl(bool isQVariant,
                                            const QObject *recever,
                                            QtPrivate::QSlotObjectBase *callback) noexcept
{
    d->isQVariant = isQVariant;
    d->recever = recever;
    d->callback = callback;
    setAsyncCall(true);
    return *this;
}

McCppResponse &McCppResponse::errorImpl(bool isQVariant,
                                        const QObject *recever,
                                        QtPrivate::QSlotObjectBase *func) noexcept
{
    d->isQVariant = isQVariant;
    d->recever = recever;
    d->error = func;
    setAsyncCall(false);
    return *this;
}

void McCppResponse::call(QtPrivate::QSlotObjectBase *func) noexcept
{
    McScopedFunction cleanup([this]() { this->deleteLater(); });
    Q_UNUSED(cleanup)

    if (func == nullptr) {
        return;
    }
    auto body = this->body();
    void *bodyStar = nullptr;
    if (d->isQVariant) {
        bodyStar = &body;
    } else {
        bodyStar = body.data();
    }
    void *args[] = {nullptr, bodyStar};
    func->call(const_cast<QObject *>(d->recever), args);
}
