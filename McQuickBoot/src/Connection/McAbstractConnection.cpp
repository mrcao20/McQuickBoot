#include "McBoot/Connection/McAbstractConnection.h"

#include <QDebug>
#include <QMetaMethod>

MC_DECL_PRIVATE_DATA(McAbstractConnection)
QObject *bean{nullptr};
QMetaMethod sig;
MC_DECL_PRIVATE_DATA_END

McAbstractConnection::McAbstractConnection(QObject *parent) noexcept : QObject(parent)
{
    MC_NEW_PRIVATE_DATA(McAbstractConnection);
}

McAbstractConnection::~McAbstractConnection()
{
    if (d->bean == nullptr) {
        return;
    }
    QMetaObject::disconnect(d->bean, d->sig.methodIndex(), this, metaObject()->methodCount());
}

int McAbstractConnection::qt_metacall(QMetaObject::Call c, int id, void **arguments)
{
    id = QObject::qt_metacall(c, id, arguments);
    if (id < 0 || c != QMetaObject::InvokeMetaMethod)
        return id;
    call(arguments);
    return -1;
}

QMetaObject::Connection McAbstractConnection::init(QObject *obj,
                                                   const QString &signal,
                                                   Qt::ConnectionType type,
                                                   QMetaMethod &sig) noexcept
{
    auto senderMetaObj = obj->metaObject();
    if (signal.indexOf('(') == -1) {
        for (int i = 0; i < senderMetaObj->methodCount(); ++i) {
            auto m = senderMetaObj->method(i);
            if (m.methodType() != QMetaMethod::Signal) {
                continue;
            }
            if (m.name() != signal) {
                continue;
            }
            d->sig = m;
            break;
        }
    } else {
        auto tmp = signal;
        if (tmp.startsWith(QT_STRINGIFY(QSIGNAL_CODE))) {
            tmp.remove(0, 1);
        }
        auto index = senderMetaObj->indexOfSignal(tmp.toLocal8Bit());
        d->sig = senderMetaObj->method(index);
    }
    if (!d->sig.isValid()) {
        qWarning() << "cannot found signal:" << signal
                   << "from class:" << senderMetaObj->className();
        return QMetaObject::Connection();
    }
    d->bean = obj;

    sig = d->sig;
    return QMetaObject::connect(obj, d->sig.methodIndex(), this, metaObject()->methodCount(), type);
}
