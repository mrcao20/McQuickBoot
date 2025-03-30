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

#include <McCore/Utils/McSlotObjectWrapper.h>

#include "../McQuickBootGlobal.h"
#include "McCppPromise.h"

namespace McPrivate {
MC_QUICKBOOT_EXPORT QVariant fail(const QString &val) noexcept;
#ifdef MC_USE_QT5
MC_QUICKBOOT_EXPORT QVariant syncInvoke(
    const McSlotObjectWrapper &functor, const QList<int> &metaTypes, const QVariantList &arguments) noexcept;
MC_QUICKBOOT_EXPORT void invoke(McAbstractPromise *promise, const McSlotObjectWrapper &functor,
    const QList<int> &metaTypes, const QVariantList &arguments) noexcept;
#else
MC_QUICKBOOT_EXPORT QVariant syncInvoke(
    const McSlotObjectWrapper &functor, const QList<QMetaType> &metaTypes, const QVariantList &arguments) noexcept;
MC_QUICKBOOT_EXPORT void invoke(McAbstractPromise *promise, const McSlotObjectWrapper &functor,
    const QList<QMetaType> &metaTypes, const QVariantList &arguments) noexcept;
#endif

template<typename R, typename Func, typename... Args>
inline R &invokeObject(
    const typename QtPrivate::FunctionPointer<Func>::Object *receiver, Func callback, const Args &...args) noexcept
{
    QVariantList arguments;
    //! 去除安卓上的警告
    Q_UNUSED((arguments << ... << McPrivate::toQVariant(args)));
    auto functor = McSlotObjectWrapper::build(receiver, callback);

    auto promise = new R(); //!< 没有指定父对象，该对象将在整个请求完毕时被析构
    invoke(promise, functor, functor.metaTypes(), arguments);
    return *promise;
}

template<typename R, typename Func, typename... Args>
inline R &invokePlain(Func callback, const Args &...args) noexcept
{
    QVariantList arguments;
    //! 去除安卓上的警告
    Q_UNUSED((arguments << ... << McPrivate::toQVariant(args)));
    auto functor = McSlotObjectWrapper::build(nullptr, callback);

    auto promise = new R(); //!< 没有指定父对象，该对象将在整个请求完毕时被析构
    invoke(promise, functor, functor.metaTypes(), arguments);
    return *promise;
}

namespace detail {
template<typename T, typename = void>
struct is_object_invoke : std::false_type
{
};
template<typename T>
struct is_object_invoke<T, std::void_t<typename QtPrivate::FunctionPointer<T>::Object>> : std::true_type
{
};
} // namespace detail
} // namespace McPrivate

namespace Mc {
template<typename Func>
inline McCppPromise &invoke(Func callback) noexcept
{
    return McPrivate::invokePlain<McCppPromise, Func>(callback);
}

template<typename Class, typename Func, typename... Args>
inline std::enable_if_t<McPrivate::detail::is_object_invoke<Func>::value
                            && std::is_same_v<std::remove_cv_t<std::remove_reference_t<std::remove_pointer_t<Class>>>,
                                typename QtPrivate::FunctionPointer<Func>::Object>,
    McCppPromise &>
    invoke(const Class *receiver, Func callback, const Args &...args) noexcept
{
    return McPrivate::invokeObject<McCppPromise, Func, Args...>(receiver, callback, args...);
}

template<typename Func, typename Arg1, typename... Args>
inline std::enable_if_t<!McPrivate::detail::is_object_invoke<Arg1>::value, McCppPromise &> invoke(
    Func callback, const Arg1 &arg1, const Args &...args) noexcept
{
    return McPrivate::invokePlain<McCppPromise, Func, Arg1, Args...>(callback, arg1, args...);
}
} // namespace Mc
