#include "McBoot/Controller/impl/McAbstractResponse.h"

#include <QCoreApplication>
#include <QEvent>
#include <QPointer>
#include <QVariant>

#include <McIoc/Utils/McScopedFunction.h>

#include "McBoot/Controller/impl/McResult.h"
#include "McBoot/Utils/Response/IMcResponseHandler.h"

MC_DECL_PRIVATE_DATA(McAbstractResponse)
bool isAsyncCall{false}; //! 是否在次线程调用callback，默认不需要
McCancel cancel;
McProgress progress;
QVariant body;
QPointer<QObject> attachedObject;
bool isAttached{false};
QList<IMcResponseHandlerPtr> responseHanlders;
MC_DECL_PRIVATE_DATA_END

MC_INIT(McAbstractResponse)
MC_INIT_END

McAbstractResponse::McAbstractResponse(QObject *parent) : QObject(parent)
{
    MC_NEW_PRIVATE_DATA(McAbstractResponse);
}

McAbstractResponse::~McAbstractResponse() {}

void McAbstractResponse::setHandlers(const QList<IMcResponseHandlerPtr> &val) noexcept
{
    d->responseHanlders = val;
}

void McAbstractResponse::cancel() noexcept
{
    d->cancel.cancel();
}

bool McAbstractResponse::isCanceled() const noexcept
{
    return d->cancel.isCanceled();
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

void McAbstractResponse::attach(QObject *obj) noexcept
{
    d->isAttached = true;
    d->attachedObject = obj;
}

void McAbstractResponse::detach() noexcept
{
    d->isAttached = false;
    d->attachedObject = nullptr;
}

void McAbstractResponse::customEvent(QEvent *event)
{
    if (event->type() == QEvent::Type::User + 1) {
        call();
    }
}

McProgress &McAbstractResponse::getProgress() const noexcept
{
    return d->progress;
}

McCancel &McAbstractResponse::getCancel() const noexcept
{
    return d->cancel;
}

void McAbstractResponse::call() noexcept
{
    McScopedFunction cleanup([this]() { this->deleteLater(); });
    Q_UNUSED(cleanup)

    for (const auto &handler : qAsConst(d->responseHanlders)) {
        if (!handler->handler(d->body)) {
            return;
        }
    }

    if (d->isAttached && d->attachedObject.isNull()) {
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
