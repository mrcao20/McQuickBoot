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
    McSlotObjectWrapper() noexcept;
#ifdef MC_USE_QT5
    McSlotObjectWrapper(const QObject *receiver, const QList<int> &qmetaTypes, int returnMetaType,
        QtPrivate::QSlotObjectBase *method) noexcept;
#else
    McSlotObjectWrapper(const QObject *receiver, const QList<QMetaType> &qmetaTypes, QMetaType returnMetaType,
        QtPrivate::QSlotObjectBase *method) noexcept;
#endif
    ~McSlotObjectWrapper();
    McSlotObjectWrapper(const McSlotObjectWrapper &o) noexcept;
    McSlotObjectWrapper &operator=(const McSlotObjectWrapper &o) noexcept;
    McSlotObjectWrapper(McSlotObjectWrapper &&o) noexcept;
    McSlotObjectWrapper &operator=(McSlotObjectWrapper &&o) noexcept;

    const QObject *receiver() const noexcept;
#ifdef MC_USE_QT5
    QList<int> metaTypes() const noexcept;
    int returnMetaType() const noexcept;
#else
    QList<QMetaType> metaTypes() const noexcept;
    QMetaType returnMetaType() const noexcept;
#endif

    //! 包裹的函数可能会抛异常，所以此类的call方法不能加noexcept关键字。
    //! \todo 尝试在编译期判断目标函数是否抛异常，从而使用不同类的call方法，从而让编译器优化noexcept
    QVariant call(const QVariantList &varList) const;
    template<typename... Args>
    QVariant call(const Args &...args) const
    {
        QVariantList vars;
        (vars << ... << McPrivate::toQVariant(args));
        return call(vars);
    }
    void call(void **arguments) const;
    template<typename R, typename... Args>
    R callPlain(const Args &...args) const
    {
        auto argSize = sizeof...(Args);
        void **arguments = new void *[argSize + 1];
        qsizetype index = 1;
        auto func = [&arguments, &index](
                        const auto &arg) { arguments[index++] = const_cast<void *>((const void *)&arg); };
        (..., func(args));
        if constexpr (std::is_void_v<R>) {
            arguments[0] = nullptr;
            call(arguments);
            delete[] arguments;
            return;
        } else {
            R r;
            arguments[0] = &r;
            call(arguments);
            delete[] arguments;
            return r;
        }
    }

    template<typename Func>
    static McSlotObjectWrapper build(
        const typename QtPrivate::FunctionPointer<Func>::Object *receiver, Func method) noexcept
    {
        typedef QtPrivate::FunctionPointer<Func> FuncType;
#ifdef MC_USE_QT5
        int returnMetaType = qMetaTypeId<typename FuncType::ReturnType>();
#else
        constexpr QMetaType returnMetaType = QMetaType::fromType<typename FuncType::ReturnType>();
#endif
        return McSlotObjectWrapper(receiver, McPrivate::MetaTypeHelper<typename FuncType::Arguments>::metaTypes(),
            returnMetaType,
            new QtPrivate::QSlotObject<Func, typename FuncType::Arguments, typename FuncType::ReturnType>(method));
    }

    template<typename Func>
    static std::enable_if_t<int(QtPrivate::FunctionPointer<Func>::ArgumentCount) >= 0
                                && !QtPrivate::FunctionPointer<Func>::IsPointerToMemberFunction,
        McSlotObjectWrapper>
        build(const QObject *context, Func functor) noexcept
    {
        typedef QtPrivate::FunctionPointer<Func> FuncType;
#ifdef MC_USE_QT5
        int returnMetaType = qMetaTypeId<typename FuncType::ReturnType>();
#else
        constexpr QMetaType returnMetaType = QMetaType::fromType<typename FuncType::ReturnType>();
#endif
        return McSlotObjectWrapper(context, McPrivate::MetaTypeHelper<typename FuncType::Arguments>::metaTypes(),
            returnMetaType,
            new QtPrivate::QStaticSlotObject<Func, typename FuncType::Arguments, typename FuncType::ReturnType>(
                functor));
    }

    template<typename Func>
    static std::enable_if_t<QtPrivate::FunctionPointer<Func>::ArgumentCount == -1, McSlotObjectWrapper> build(
        const QObject *context, Func functor) noexcept
    {
        typedef McPrivate::LambdaType<Func> FuncType;
#ifdef MC_USE_QT5
        int returnMetaType = qMetaTypeId<typename FuncType::ReturnType>();
#else
        constexpr QMetaType returnMetaType = QMetaType::fromType<typename FuncType::ReturnType>();
#endif
        return McSlotObjectWrapper(context, McPrivate::MetaTypeHelper<typename FuncType::Arguments>::metaTypes(),
            returnMetaType,
            new QtPrivate::QFunctorSlotObject<Func, int(FuncType::ArgumentCount), typename FuncType::Arguments,
                typename FuncType::ReturnType>(std::move(functor)));
    }

    template<typename Func>
    static McSlotObjectWrapper buildPlain(
        const typename QtPrivate::FunctionPointer<Func>::Object *receiver, Func method) noexcept
    {
        typedef QtPrivate::FunctionPointer<Func> FuncType;
#ifdef MC_USE_QT5
        return McSlotObjectWrapper(receiver, QList<int>(), -1,
            new QtPrivate::QSlotObject<Func, typename FuncType::Arguments, typename FuncType::ReturnType>(method));
#else
        return McSlotObjectWrapper(receiver, QList<QMetaType>(), QMetaType(),
            new QtPrivate::QSlotObject<Func, typename FuncType::Arguments, typename FuncType::ReturnType>(method));
#endif
    }

    template<typename Func>
    static std::enable_if_t<int(QtPrivate::FunctionPointer<Func>::ArgumentCount) >= 0
                                && !QtPrivate::FunctionPointer<Func>::IsPointerToMemberFunction,
        McSlotObjectWrapper>
        buildPlain(const QObject *context, Func functor) noexcept
    {
        typedef QtPrivate::FunctionPointer<Func> FuncType;
#ifdef MC_USE_QT5
        return McSlotObjectWrapper(context, QList<int>(), -1,
            new QtPrivate::QStaticSlotObject<Func, typename FuncType::Arguments, typename FuncType::ReturnType>(
                functor));
#else
        return McSlotObjectWrapper(context, QList<QMetaType>(), QMetaType(),
            new QtPrivate::QStaticSlotObject<Func, typename FuncType::Arguments, typename FuncType::ReturnType>(
                functor));
#endif
    }

    template<typename Func>
    static std::enable_if_t<QtPrivate::FunctionPointer<Func>::ArgumentCount == -1, McSlotObjectWrapper> buildPlain(
        const QObject *context, Func functor) noexcept
    {
        typedef McPrivate::LambdaType<Func> FuncType;
#ifdef MC_USE_QT5
        return McSlotObjectWrapper(context, QList<int>(), -1,
            new QtPrivate::QFunctorSlotObject<Func, int(FuncType::ArgumentCount), typename FuncType::Arguments,
                typename FuncType::ReturnType>(std::move(functor)));
#else
        return McSlotObjectWrapper(context, QList<QMetaType>(), QMetaType(),
            new QtPrivate::QFunctorSlotObject<Func, int(FuncType::ArgumentCount), typename FuncType::Arguments,
                typename FuncType::ReturnType>(std::move(functor)));
#endif
    }

private:
    MC_DECL_COPYABLE_PRIVATE(McSlotObjectWrapper);
};
