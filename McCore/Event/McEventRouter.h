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

#include <QHash>
#include <QObject>

#include "../Utils/McSlotObjectWrapper.h"

class McEventRouter : public QObject
{
    Q_OBJECT
public:
    explicit McEventRouter(QObject *parent = nullptr) noexcept;

    QMetaObject::Connection connectToEvent(
        const QStringList &segments, const QObject *receiver, const char *method, Qt::ConnectionType type) noexcept;
    QMetaObject::Connection connectToEvent(
        const QStringList &segments, const McSlotObjectWrapper &slotObject, Qt::ConnectionType type) noexcept;

    bool disconnectEvent(const QStringList &segments, const QObject *receiver, const char *method) noexcept;
    bool disconnectEvent(const QStringList &segments, const QObject *receiver) noexcept;

    void route(const QStringList &segments, const QVariantList &datas) noexcept;

Q_SIGNALS:
    void eventOccurred(const QVariantList &datas);

private:
    McEventRouter *child(const QString &segment) noexcept;

private:
    QHash<QString, McEventRouter *> m_children;
};
