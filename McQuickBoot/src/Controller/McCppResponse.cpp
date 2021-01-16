#include "McBoot/Controller/impl/McCppResponse.h"

#include <QQmlEngine>
#include <QVariant>

#include <McIoc/Utils/McScopedFunction.h>

#include "McBoot/Controller/impl/McResult.h"

MC_DECL_PRIVATE_DATA(McCppResponse)
std::function<void(const QVariant &)> callback;
MC_DECL_PRIVATE_DATA_END

MC_INIT(McCppResponse)
MC_INIT_END

McCppResponse::McCppResponse(QObject *parent) : McAbstractResponse(parent)
{
    MC_NEW_PRIVATE_DATA(McCppResponse);
}

McCppResponse::~McCppResponse() {}

McCppResponse &McCppResponse::then(const std::function<void(const QVariant &)> &callback) noexcept
{
    d->callback = callback;
    setAsyncCall(false);
    return *this;
}

McCppResponse &McCppResponse::syncThen(const std::function<void(const QVariant &)> &callback) noexcept
{
    d->callback = callback;
    setAsyncCall(false);
    return *this;
}

McCppResponse &McCppResponse::asyncThen(const std::function<void(const QVariant &)> &callback) noexcept
{
    d->callback = callback;
    setAsyncCall(true);
    return *this;
}

void McCppResponse::callCallback() noexcept
{
    McScopedFunction func([this]() { this->deleteLater(); });
    Q_UNUSED(func)

    if (!d->callback) {
        return;
    }
    d->callback(body());
}
