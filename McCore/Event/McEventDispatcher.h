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

#include <QVariant>

#include "../Utils/McSlotObjectWrapper.h"

MC_FORWARD_DECL_PRIVATE_DATA(McEventDispatcher)

class MC_CORE_EXPORT McEventDispatcher
{
    Q_DISABLE_COPY_MOVE(McEventDispatcher)

public:
    McEventDispatcher() noexcept;
    ~McEventDispatcher();

    //! method只能是无参或者参数是QVariantList
    QMetaObject::Connection connectToEvent(const QString &eventSpec, const QObject *receiver, const char *method,
        Qt::ConnectionType type = Qt::AutoConnection) noexcept;

    template<typename Object, typename Func>
    inline std::enable_if_t<!std::is_same_v<const char *, Func>, QMetaObject::Connection> connectToEvent(
        const QString &eventSpec, const Object *receiver, Func method,
        Qt::ConnectionType type = Qt::AutoConnection) noexcept
    {
        return connectToEventImpl(eventSpec, McSlotObjectWrapper::build(receiver, method), type);
    }

    template<typename Func>
    inline std::enable_if_t<!std::is_same_v<const char *, Func>, QMetaObject::Connection> connectToEvent(
        const QString &eventSpec, Func method, Qt::ConnectionType type = Qt::AutoConnection) noexcept
    {
        return connectToEventImpl(eventSpec, McSlotObjectWrapper::build(nullptr, std::move(method)), type);
    }

    bool disconnectEvent(const QString &eventSpec, const QObject *receiver, const char *method) noexcept;
    bool disconnectEvent(const QString &eventSpec, const QObject *receiver = nullptr) noexcept;

    template<typename... Args>
    void submitEvent(const QString &eventName, Args &&...args) noexcept
    {
        QVariantList vars;
        //! 去除安卓上的警告
        Q_UNUSED((vars << ... << McPrivate::toQVariant(std::forward<Args>(args))));
        submitEvent_helper(eventName, vars);
    }

private:
    void submitEvent_helper(const QString &eventName, const QVariantList &datas) noexcept;
    QMetaObject::Connection connectToEventImpl(const QString &eventSpec, const McSlotObjectWrapper &slotObject,
        Qt::ConnectionType type = Qt::AutoConnection) noexcept;

private:
    MC_DECL_PRIVATE(McEventDispatcher)
};

namespace Mc {
MC_CORE_EXPORT McEventDispatcher &eventDispatcher() noexcept;
}

#define mcEvt() Mc::eventDispatcher()
