#include "McBoot/Controller/impl/McAbstractResponse.h"

#include <QCoreApplication>
#include <QEvent>
#include <QVariant>

MC_DECL_PRIVATE_DATA(McAbstractResponse)
bool isAsyncCall{false}; // 是否在次线程调用callback，默认不需要
QVariant body;
MC_DECL_PRIVATE_DATA_END

MC_INIT(McAbstractResponse)
MC_INIT_END

McAbstractResponse::McAbstractResponse(QObject *parent) : QObject(parent)
{
    MC_NEW_PRIVATE_DATA(McAbstractResponse);
}

McAbstractResponse::~McAbstractResponse() {}

QVariant McAbstractResponse::body() const noexcept
{
    return d->body;
}

void McAbstractResponse::setBody(const QVariant &var) noexcept
{
    d->body = var;

    if (isAsyncCall()) {
        callCallback();
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
        callCallback();
    }
}
