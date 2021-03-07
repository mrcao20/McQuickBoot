#include "McBoot/Connection/McCppConnection.h"

#include <QDebug>
#include <QMetaMethod>

MC_DECL_PRIVATE_DATA(McCppConnection)
QObject *receiver{nullptr};
QMetaMethod sig;
QtPrivate::QSlotObjectBase *callback{nullptr};
MC_DECL_PRIVATE_DATA_END

McCppConnection::McCppConnection(QObject *parent) noexcept : McAbstractConnection(parent)
{
    MC_NEW_PRIVATE_DATA(McCppConnection);
}

McCppConnection::~McCppConnection()
{
    d->callback->destroyIfLastRef();
}

QMetaObject::Connection McCppConnection::init(QObject *sender,
                                              const QString &signal,
                                              QObject *receiver,
                                              QtPrivate::QSlotObjectBase *slot,
                                              Qt::ConnectionType type) noexcept
{
    d->callback = slot;
    auto con = McAbstractConnection::init(sender, signal, type, d->sig);
    if (!d->sig.isValid()) {
        return con;
    }
    moveToThread(receiver->thread());
    setParent(receiver);
    d->receiver = receiver;
    return con;
}

void McCppConnection::call(void **arguments) noexcept
{
    d->callback->call(d->receiver, arguments);
}
