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
#pragma once

#include <QVariant>

#include "../McMacroGlobal.h"
#include "../Utils/McSlotObjectWrapper.h"

MC_FORWARD_DECL_PRIVATE_DATA(McEventDispatcher)

class MC_CORE_EXPORT McEventDispatcher
{
    McEventDispatcher() noexcept;
    Q_DISABLE_COPY_MOVE(McEventDispatcher)

public:
    ~McEventDispatcher();

    static McEventDispatcher *instance() noexcept;

    QMetaObject::Connection connectToEvent(const QString &scxmlEventSpec, const QObject *receiver, const char *method,
        Qt::ConnectionType type = Qt::AutoConnection) noexcept;

    template<typename Func>
    inline QMetaObject::Connection connectToEvent(const QString &scxmlEventSpec,
        const typename QtPrivate::FunctionPointer<Func>::Object *receiver, Func method,
        Qt::ConnectionType type = Qt::AutoConnection) noexcept
    {
        static_assert(int(QtPrivate::FunctionPointer<Func>::ArgumentCount) <= 1,
            "The number of parameters of callback function can only be less than or equal to 1");

        return connectToEventImpl(scxmlEventSpec, McSlotObjectWrapper::build(receiver, method), type);
    }

    template<typename Func>
    inline std::enable_if_t<int(QtPrivate::FunctionPointer<Func>::ArgumentCount) >= 0
                                && !QtPrivate::FunctionPointer<Func>::IsPointerToMemberFunction
                                && !std::is_same<const char *, Func>::value,
        QMetaObject::Connection>
        connectToEvent(
            const QString &scxmlEventSpec, Func functor, Qt::ConnectionType type = Qt::AutoConnection) noexcept
    {
        return connectToEvent(scxmlEventSpec, nullptr, functor, type);
    }

    template<typename Func>
    inline std::enable_if_t<int(QtPrivate::FunctionPointer<Func>::ArgumentCount) >= 0
                                && !QtPrivate::FunctionPointer<Func>::IsPointerToMemberFunction
                                && !std::is_same<const char *, Func>::value,
        QMetaObject::Connection>
        connectToEvent(const QString &scxmlEventSpec, const QObject *context, Func functor,
            Qt::ConnectionType type = Qt::AutoConnection) noexcept
    {
        static_assert(int(QtPrivate::FunctionPointer<Func>::ArgumentCount) <= 1,
            "The number of parameters of callback function can only be less than or equal to 1");

        return connectToEventImpl(scxmlEventSpec, McSlotObjectWrapper::build(context, functor), type);
    }

    template<typename Func>
    inline std::enable_if_t<QtPrivate::FunctionPointer<Func>::ArgumentCount == -1
                                && !QtPrivate::FunctionPointer<Func>::IsPointerToMemberFunction
                                && !std::is_same<const char *, Func>::value,
        QMetaObject::Connection>
        connectToEvent(
            const QString &scxmlEventSpec, Func functor, Qt::ConnectionType type = Qt::AutoConnection) noexcept
    {
        return connectToEvent(scxmlEventSpec, nullptr, std::move(functor), type);
    }

    template<typename Func>
    inline std::enable_if_t<QtPrivate::FunctionPointer<Func>::ArgumentCount == -1
                                && !QtPrivate::FunctionPointer<Func>::IsPointerToMemberFunction
                                && !std::is_same<const char *, Func>::value,
        QMetaObject::Connection>
        connectToEvent(const QString &scxmlEventSpec, const QObject *context, Func functor,
            Qt::ConnectionType type = Qt::AutoConnection) noexcept
    {
        static_assert(int(McPrivate::LambdaType<Func>::ArgumentCount) <= 1,
            "The number of parameters of callback function can only be less than or equal to 1");

        return connectToEventImpl(scxmlEventSpec, McSlotObjectWrapper::build(context, std::move(functor)), type);
    }

    void submitEvent(const QString &eventName) noexcept;
    template<typename T>
    void submitEvent(const QString &eventName, T &&data) noexcept
    {
        submitEvent_helper(eventName, McPrivate::toQVariant(std::forward<T>(data)));
    }

private:
    void submitEvent_helper(const QString &eventName, const QVariant &data) noexcept;
    QMetaObject::Connection connectToEventImpl(const QString &scxmlEventSpec, const McSlotObjectWrapper &slotObject,
        Qt::ConnectionType type = Qt::AutoConnection) noexcept;

private:
    MC_DECL_PRIVATE(McEventDispatcher)
};

namespace Mc {
MC_CORE_EXPORT McEventDispatcher &eventDispatcher() noexcept;
}

#define mcEvt() Mc::eventDispatcher()
