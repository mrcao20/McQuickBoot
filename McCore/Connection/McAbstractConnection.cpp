/*
 * MIT License
 *
 * Copyright (c) 2021 mrcao20
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
