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
#pragma once

#include "../Utils/McSlotObjectWrapper.h"
#include "McAbstractConnection.h"

MC_FORWARD_DECL_PRIVATE_DATA(McCppConnection)

class McCppConnection : public McAbstractConnection
{
public:
    explicit McCppConnection(QObject *parent = nullptr) noexcept;
    ~McCppConnection() override;

    QMetaObject::Connection init(
        QObject *sender, const QString &signal, const McSlotObjectWrapper &slot, Qt::ConnectionType type) noexcept;

protected:
    using McAbstractConnection::init;
    void call(void **arguments) noexcept override;

private:
    MC_DECL_PRIVATE(McCppConnection)
};
