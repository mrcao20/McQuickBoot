/*
 * Copyright (c) 2021 mrcao20/mrcao20@163.com
 * McQuickBoot is licensed under Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at:
 *          http://license.coscl.org.cn/MulanPSL2
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
 * EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
 * MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
 * See the Mulan PSL v2 for more details.
 */
#include "McAbstractConnection.h"

#include <QDebug>
#include <QMetaMethod>
#include <QPointer>

MC_DECL_PRIVATE_DATA(McAbstractConnection)
QPointer<QObject> bean;
QMetaMethod sig;
MC_DECL_PRIVATE_DATA_END

McAbstractConnection::McAbstractConnection(QObject *parent) noexcept
    : QObject(parent)
{
    MC_NEW_PRIVATE_DATA(McAbstractConnection);
}

McAbstractConnection::~McAbstractConnection()
{
    if (d->bean.isNull()) {
        return;
    }
    QMetaObject::disconnect(d->bean.data(), d->sig.methodIndex(), this, metaObject()->methodCount());
}

int McAbstractConnection::qt_metacall(QMetaObject::Call c, int id, void **arguments)
{
    id = QObject::qt_metacall(c, id, arguments);
    if (id < 0 || c != QMetaObject::InvokeMetaMethod) {
        return id;
    }
    call(arguments);
    return -1;
}

QMetaObject::Connection McAbstractConnection::init(
    QObject *obj, const QString &signal, Qt::ConnectionType type, QMetaMethod &sig) noexcept
{
    if (obj == nullptr) {
        return QMetaObject::Connection();
    }
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
        qCWarning(mcCore()) << "cannot found signal:" << signal << "from class:" << senderMetaObj->className();
        return QMetaObject::Connection();
    }
    QPointer<QObject> p(obj);
    d->bean.swap(p);

    sig = d->sig;
    return QMetaObject::connect(obj, d->sig.methodIndex(), this, metaObject()->methodCount(), type);
}
