#include "McBoot/Controller/impl/McAbstractResponse.h"

#include <QCoreApplication>
#include <QEvent>
#include <QVariant>

#include <McIoc/Utils/McScopedFunction.h>

#include "McBoot/Controller/impl/McResult.h"
#include "McBoot/McBootGlobal.h"

MC_DECL_PRIVATE_DATA(McAbstractResponse)
bool isAsyncCall{false}; // 是否在次线程调用callback，默认不需要
bool isCancel{false};
QVariant body;
MC_DECL_PRIVATE_DATA_END

MC_INIT(McAbstractResponse)
MC_INIT_END

McAbstractResponse::McAbstractResponse(QObject *parent) : QObject(parent)
{
    MC_NEW_PRIVATE_DATA(McAbstractResponse);
}

McAbstractResponse::~McAbstractResponse() {}

void McAbstractResponse::cancel() noexcept
{
    d->isCancel = true;
}

bool McAbstractResponse::isCancel() const noexcept
{
    return d->isCancel;
}

QVariant McAbstractResponse::body() const noexcept
{
    return d->body;
}

void McAbstractResponse::setBody(const QVariant &var) noexcept
{
    d->body = var;

    if (isAsyncCall()) {
        call();
        return;
    }

    //! 发布的事件由QT删除
    qApp->postEvent(this, new QEvent(static_cast<QEvent::Type>(QEvent::Type::User + 1)));
}

bool McAbstractResponse::isAsyncCall() const noexcept
{
    return d->isAsyncCall;
}

void McAbstractResponse::setAsyncCall(bool val) noexcept
{
    d->isAsyncCall = val;
}

void McAbstractResponse::customEvent(QEvent *event)
{
    if (event->type() == QEvent::Type::User + 1) {
        call();
    }
}

void McAbstractResponse::call() noexcept
{
    McScopedFunction cleanup([this]() { this->deleteLater(); });
    Q_UNUSED(cleanup)

    if (d->isCancel) {
        return;
    }

    if (!d->body.canConvert<McResultPtr>()) {
        callCallback();
        return;
    }
    auto result = d->body.value<McResultPtr>();
    if (result->isInternalError()) {
        qCWarning(mcQuickBoot()) << result;
        callError();
    } else {
        callCallback();
    }
}
