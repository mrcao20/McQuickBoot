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

#include "../McGlobal.h"

MC_FORWARD_DECL_PRIVATE_DATA(McSlotObjectWrapper)

class MC_CORE_EXPORT McSlotObjectWrapper
{
public:
#ifdef MC_USE_QT5
    McSlotObjectWrapper(
        const QObject *recever, const QList<int> &qmetaTypes, QtPrivate::QSlotObjectBase *method) noexcept;
#else
    McSlotObjectWrapper(
        const QObject *recever, const QList<QMetaType> &qmetaTypes, QtPrivate::QSlotObjectBase *method) noexcept;
#endif
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
