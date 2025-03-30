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

#include <QObject>

#include "../McGlobal.h"

MC_FORWARD_DECL_PRIVATE_DATA(McAbstractConnection)

class McAbstractConnection : public QObject
{
public:
    explicit McAbstractConnection(QObject *parent = nullptr) noexcept;
    ~McAbstractConnection() override;

    int qt_metacall(QMetaObject::Call c, int id, void **arguments) override final;

protected:
    virtual void call(void **arguments) noexcept = 0;

protected:
    QMetaObject::Connection init(
        QObject *obj, const QString &signal, Qt::ConnectionType type, QMetaMethod &sig) noexcept;

private:
    MC_DECL_PRIVATE(McAbstractConnection)
};
