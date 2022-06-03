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

#include "../McGlobal.h"

MC_FORWARD_DECL_PRIVATE_DATA(McSlotObjectWrapper)

class MC_CORE_EXPORT McSlotObjectWrapper
{
public:
    McSlotObjectWrapper(
        const QObject *recever, const QList<QMetaType> &qmetaTypes, QtPrivate::QSlotObjectBase *method) noexcept;
    ~McSlotObjectWrapper();
    McSlotObjectWrapper(const McSlotObjectWrapper &o) noexcept;
    McSlotObjectWrapper &operator=(const McSlotObjectWrapper &o) noexcept;
    McSlotObjectWrapper(McSlotObjectWrapper &&o) noexcept;
    McSlotObjectWrapper &operator=(McSlotObjectWrapper &&o) noexcept;

    const QObject *recever() const noexcept;

    void call(const QVariant &var) const noexcept { call(QVariantList() << var); }
    void call(const QVariantList &varList) const noexcept;

    template<typename Func>
    static McSlotObjectWrapper build(
        const typename QtPrivate::FunctionPointer<Func>::Object *receiver, Func method) noexcept
    {
        typedef QtPrivate::FunctionPointer<Func> FuncType;

        return McSlotObjectWrapper(receiver, McPrivate::MetaTypeHelper<typename FuncType::Arguments>::metaTypes(),
            new QtPrivate::QSlotObject<Func, typename FuncType::Arguments, typename FuncType::ReturnType>(method));
    }

    template<typename Func>
    static std::enable_if_t<int(QtPrivate::FunctionPointer<Func>::ArgumentCount) >= 0
                                && !QtPrivate::FunctionPointer<Func>::IsPointerToMemberFunction,
        McSlotObjectWrapper>
        build(const QObject *context, Func functor) noexcept
    {
        typedef QtPrivate::FunctionPointer<Func> FuncType;

        return McSlotObjectWrapper(context, McPrivate::MetaTypeHelper<typename FuncType::Arguments>::metaTypes(),
            new QtPrivate::QStaticSlotObject<Func, typename FuncType::Arguments, typename FuncType::ReturnType>(
                functor));
    }

    template<typename Func>
    static std::enable_if_t<QtPrivate::FunctionPointer<Func>::ArgumentCount == -1, McSlotObjectWrapper> build(
        const QObject *context, Func functor) noexcept
    {
        typedef McPrivate::LambdaType<Func> FuncType;

        return McSlotObjectWrapper(context, McPrivate::MetaTypeHelper<typename FuncType::Arguments>::metaTypes(),
            new QtPrivate::QFunctorSlotObject<Func, int(FuncType::ArgumentCount), typename FuncType::Arguments,
                typename FuncType::ReturnType>(std::move(functor)));
    }

private:
    MC_DECL_COPYABLE_PRIVATE(McSlotObjectWrapper);
};
