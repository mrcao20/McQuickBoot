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
#include "../Utils/McQVariantConverter.h"

MC_FORWARD_DECL_PRIVATE_DATA(McEventDispatcher)

class MC_CORE_EXPORT McEventDispatcher : public QObject, protected McQVariantConverter
{
    Q_OBJECT
    McEventDispatcher() noexcept;
    Q_DISABLE_COPY_MOVE(McEventDispatcher)

public:
    ~McEventDispatcher();

    static McEventDispatcher *instance() noexcept;

    QMetaObject::Connection connectToEvent(const QString &scxmlEventSpec,
                                           const QObject *receiver,
                                           const char *method,
                                           Qt::ConnectionType type = Qt::AutoConnection) noexcept;

    // connect state to a QObject slot
    template<typename PointerToMemberFunction>
    inline QMetaObject::Connection connectToEvent(
        const QString &scxmlEventSpec,
        const typename QtPrivate::FunctionPointer<PointerToMemberFunction>::Object *receiver,
        PointerToMemberFunction method,
        Qt::ConnectionType type = Qt::AutoConnection) noexcept
    {
        typedef QtPrivate::FunctionPointer<PointerToMemberFunction> SlotType;
        return connectToEventImpl(
            scxmlEventSpec,
            receiver,
            nullptr,
            new QtPrivate::QSlotObject<PointerToMemberFunction, typename SlotType::Arguments, void>(
                method),
            type);
    }

    // connect state to a functor or function pointer (without context)
    template<typename Functor>
    inline typename std::enable_if<!QtPrivate::FunctionPointer<Functor>::IsPointerToMemberFunction
                                       && !std::is_same<const char *, Functor>::value,
                                   QMetaObject::Connection>::type
    connectToEvent(const QString &scxmlEventSpec, Functor functor, Qt::ConnectionType type = Qt::AutoConnection) noexcept
    {
        // Use this as context
        return connectToEvent(scxmlEventSpec, this, functor, type);
    }

    // connectToEvent to a functor or function pointer (with context)
    template<typename Functor>
    inline typename std::enable_if<!QtPrivate::FunctionPointer<Functor>::IsPointerToMemberFunction
                                       && !std::is_same<const char *, Functor>::value,
                                   QMetaObject::Connection>::type
    connectToEvent(const QString &scxmlEventSpec,
                   const QObject *context,
                   Functor functor,
                   Qt::ConnectionType type = Qt::AutoConnection) noexcept
    {
        QtPrivate::QSlotObjectBase *slotObj
            = new QtPrivate::QFunctorSlotObject<Functor, 1, QtPrivate::List<QVariant>, void>(
                functor);
        return connectToEventImpl(scxmlEventSpec,
                                  context,
                                  reinterpret_cast<void **>(&functor),
                                  slotObj,
                                  type);
    }

    void submitEvent(const QString &eventName) noexcept;
    template<typename T>
    void submitEvent(const QString &eventName, T &&data) noexcept
    {
        submitEvent_helper(eventName, toQVariant(data));
    }

private:
    void submitEvent_helper(const QString &eventName, const QVariant &data) noexcept;
    QMetaObject::Connection connectToEventImpl(const QString &scxmlEventSpec,
                                               const QObject *receiver,
                                               void **slot,
                                               QtPrivate::QSlotObjectBase *slotObj,
                                               Qt::ConnectionType type = Qt::AutoConnection) noexcept;

private:
    MC_DECL_PRIVATE(McEventDispatcher)
};

namespace Mc {
MC_CORE_EXPORT McEventDispatcher &eventDispatcher() noexcept;
}

#define mcEvt() Mc::eventDispatcher()
