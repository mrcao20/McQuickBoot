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

#include <optional>

#include "../../Utils/McSlotObjectWrapper.h"
#include "McAbstractAsyncCallback.h"

/*!
 * \brief The McCppAsyncCallback class
 * 调用此类的call方法，回调函数将会立即执行。
 * 即回调函数的执行线程和调用此类的call方法的线程为同一个。
 */
class MC_CORE_EXPORT McCppAsyncCallback : public McAbstractAsyncCallback
{
    MC_DECL_INIT(McCppAsyncCallback)
    MC_INTERFACES(McAbstractAsyncCallback)
    MC_DECL_POINTER(McCppAsyncCallback)
public:
    using IMcCallback::call;

    McCppAsyncCallback(const McCppAsyncCallback &o) noexcept;
    McCppAsyncCallback &operator=(const McCppAsyncCallback &o) noexcept;
    McCppAsyncCallback(McCppAsyncCallback &&o) noexcept;
    McCppAsyncCallback &operator=(McCppAsyncCallback &&o) noexcept;

    QVariant call(const QVariantList &varList) const noexcept override;

    template<typename Func>
    McCppAsyncCallback(const typename QtPrivate::FunctionPointer<Func>::Object *receiver, Func callback) noexcept
    {
        m_callback = std::make_optional(McSlotObjectWrapper::build(receiver, callback));
    }

    template<typename Func>
    McCppAsyncCallback(Func callback) noexcept
    {
        m_callback = std::make_optional(McSlotObjectWrapper::build(nullptr, std::move(callback)));
    }

    template<typename Func>
    static McCppAsyncCallbackPtr build(
        const typename QtPrivate::FunctionPointer<Func>::Object *receiver, Func callback) noexcept
    {
        return McCppAsyncCallbackPtr(new McCppAsyncCallback(receiver, callback), CallbackDeleter());
    }
    template<typename Func>
    static McCppAsyncCallbackPtr build(Func callback) noexcept
    {
        return McCppAsyncCallbackPtr(new McCppAsyncCallback(std::move(callback)), CallbackDeleter());
    }

private:
    std::optional<McSlotObjectWrapper> m_callback;
};

MC_DECL_METATYPE(McCppAsyncCallback)
