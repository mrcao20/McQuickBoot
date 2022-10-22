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
MC_QUICKBOOT_EXPORT QVariant syncInvoke(const McSlotObjectWrapper &functor, const QVariantList &arguments) noexcept;
MC_QUICKBOOT_EXPORT void invoke(
    McAbstractPromise *promise, const McSlotObjectWrapper &functor, const QVariantList &arguments) noexcept;

template<typename R, typename Func, typename... Args>
inline R &invoke(
    const typename QtPrivate::FunctionPointer<Func>::Object *receiver, Func callback, const Args &...args) noexcept
{
    QVariantList arguments;
    (arguments << ... << McPrivate::toQVariant(args));
    auto functor = McSlotObjectWrapper::build(receiver, callback);

    auto promise = new R(); //!< 没有指定父对象，该对象将在整个请求完毕时被析构
    invoke(promise, functor, arguments);
    return *promise;
}

template<typename R, typename Func, typename... Args>
inline R &invoke(Func callback, const Args &...args) noexcept
{
    QVariantList arguments;
    (arguments << ... << McPrivate::toQVariant(args));
    auto functor = McSlotObjectWrapper::build(nullptr, callback);

    auto promise = new R(); //!< 没有指定父对象，该对象将在整个请求完毕时被析构
    invoke(promise, functor, arguments);
    return *promise;
}
} // namespace McPrivate

namespace Mc {
template<typename Func, typename... Args>
inline McCppPromise &invoke(
    const typename QtPrivate::FunctionPointer<Func>::Object *receiver, Func callback, const Args &...args) noexcept
{
    return McPrivate::invoke<McCppPromise, Func, Args...>(receiver, callback, args...);
}

template<typename Func, typename... Args>
inline McCppPromise &invoke(Func callback, const Args &...args) noexcept
{
    return McPrivate::invoke<McCppPromise, Func, Args...>(callback, args...);
}
}
