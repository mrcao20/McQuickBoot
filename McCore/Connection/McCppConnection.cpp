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
#include "McCppConnection.h"

#include <QMetaMethod>

MC_DECL_PRIVATE_DATA(McCppConnection)
QMetaMethod sig;
McSlotObjectWrapper callback;
MC_DECL_PRIVATE_DATA_END

McCppConnection::McCppConnection(QObject *parent) noexcept
    : McAbstractConnection(parent)
{
    MC_NEW_PRIVATE_DATA(McCppConnection);
}

McCppConnection::~McCppConnection()
{
}

QMetaObject::Connection McCppConnection::init(
    QObject *sender, const QString &signal, const McSlotObjectWrapper &slot, Qt::ConnectionType type) noexcept
{
    d->callback = slot;
    auto con = McAbstractConnection::init(sender, signal, type, d->sig);
    if (!d->sig.isValid()) {
        return con;
    }
    moveToThread(slot.receiver()->thread());
    setParent(const_cast<QObject *>(slot.receiver()));
    return con;
}

void McCppConnection::call(void **arguments) noexcept
{
    d->callback.call(arguments);
}
