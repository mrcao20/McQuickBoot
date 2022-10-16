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

#include "../McMacroGlobal.h"
#include "../Utils/McSlotObjectWrapper.h"

MC_FORWARD_DECL_PRIVATE_DATA(McEventDispatcher)

class MC_CORE_EXPORT McEventDispatcher
{
    Q_DISABLE_COPY_MOVE(McEventDispatcher)

public:
    McEventDispatcher() noexcept;
    ~McEventDispatcher();

    QMetaObject::Connection connectToEvent(const QString &eventSpec, const QObject *receiver, const char *method,
        Qt::ConnectionType type = Qt::AutoConnection) noexcept;

    template<typename Func>
    inline QMetaObject::Connection connectToEvent(const QString &eventSpec,
        const typename QtPrivate::FunctionPointer<Func>::Object *receiver, Func method,
        Qt::ConnectionType type = Qt::AutoConnection) noexcept
    {
        static_assert(int(QtPrivate::FunctionPointer<Func>::ArgumentCount) <= 1,
            "The number of parameters of callback function can only be less than or equal to 1");

        return connectToEventImpl(eventSpec, McSlotObjectWrapper::build(receiver, method), type);
    }

    template<typename Func>
    inline std::enable_if_t<int(QtPrivate::FunctionPointer<Func>::ArgumentCount) >= 0
                                && !QtPrivate::FunctionPointer<Func>::IsPointerToMemberFunction
                                && !std::is_same<const char *, Func>::value,
        QMetaObject::Connection>
        connectToEvent(const QString &eventSpec, Func functor, Qt::ConnectionType type = Qt::AutoConnection) noexcept
    {
        return connectToEvent(eventSpec, nullptr, functor, type);
    }

    template<typename Func>
    inline std::enable_if_t<int(QtPrivate::FunctionPointer<Func>::ArgumentCount) >= 0
                                && !QtPrivate::FunctionPointer<Func>::IsPointerToMemberFunction
                                && !std::is_same<const char *, Func>::value,
        QMetaObject::Connection>
        connectToEvent(const QString &eventSpec, const QObject *context, Func functor,
            Qt::ConnectionType type = Qt::AutoConnection) noexcept
    {
        static_assert(int(QtPrivate::FunctionPointer<Func>::ArgumentCount) <= 1,
            "The number of parameters of callback function can only be less than or equal to 1");

        return connectToEventImpl(eventSpec, McSlotObjectWrapper::build(context, functor), type);
    }

    template<typename Func>
    inline std::enable_if_t<QtPrivate::FunctionPointer<Func>::ArgumentCount == -1
                                && !QtPrivate::FunctionPointer<Func>::IsPointerToMemberFunction
                                && !std::is_same<const char *, Func>::value,
        QMetaObject::Connection>
        connectToEvent(const QString &eventSpec, Func functor, Qt::ConnectionType type = Qt::AutoConnection) noexcept
    {
        return connectToEvent(eventSpec, nullptr, std::move(functor), type);
    }

    template<typename Func>
    inline std::enable_if_t<QtPrivate::FunctionPointer<Func>::ArgumentCount == -1
                                && !QtPrivate::FunctionPointer<Func>::IsPointerToMemberFunction
                                && !std::is_same<const char *, Func>::value,
        QMetaObject::Connection>
        connectToEvent(const QString &eventSpec, const QObject *context, Func functor,
            Qt::ConnectionType type = Qt::AutoConnection) noexcept
    {
        static_assert(int(McPrivate::LambdaType<Func>::ArgumentCount) <= 1,
            "The number of parameters of callback function can only be less than or equal to 1");

        return connectToEventImpl(eventSpec, McSlotObjectWrapper::build(context, std::move(functor)), type);
    }

    bool disconnectEvent(const QString &eventSpec, const QObject *receiver, const char *method) noexcept;
    bool disconnectEvent(const QString &eventSpec, const QObject *receiver = nullptr) noexcept;

    void submitEvent(const QString &eventName) noexcept;
    template<typename T>
    void submitEvent(const QString &eventName, T &&data) noexcept
    {
        submitEvent_helper(eventName, McPrivate::toQVariant(std::forward<T>(data)));
    }

private:
    void submitEvent_helper(const QString &eventName, const QVariant &data) noexcept;
    QMetaObject::Connection connectToEventImpl(const QString &eventSpec, const McSlotObjectWrapper &slotObject,
        Qt::ConnectionType type = Qt::AutoConnection) noexcept;

private:
    MC_DECL_PRIVATE(McEventDispatcher)
};

namespace Mc {
MC_CORE_EXPORT McEventDispatcher &eventDispatcher() noexcept;
}

#define mcEvt() Mc::eventDispatcher()
