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
