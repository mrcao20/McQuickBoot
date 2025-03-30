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
    McCppAsyncCallback() noexcept;
    ~McCppAsyncCallback() override;

    McCppAsyncCallback(const McCppAsyncCallback &o) noexcept;
    McCppAsyncCallback &operator=(const McCppAsyncCallback &o) noexcept;
    McCppAsyncCallback(McCppAsyncCallback &&o) noexcept;
    McCppAsyncCallback &operator=(McCppAsyncCallback &&o) noexcept;

    using IMcCallback::call;
    QVariant call(const QVariantList &varList) const noexcept override;

    template<typename Object, typename Func, typename Checker = McSlotObjectWrapper::DummyChecker>
    McCppAsyncCallback(const Object *receiver, Func callback, Checker *dummy = nullptr) noexcept
    {
        Q_UNUSED(dummy)
        m_callback = std::make_optional(McSlotObjectWrapper::build<Func, Checker>(receiver, callback));
    }

    template<typename Func, typename Checker = McSlotObjectWrapper::DummyChecker>
    McCppAsyncCallback(Func callback, Checker *dummy = nullptr) noexcept
    {
        Q_UNUSED(dummy)
        m_callback = std::make_optional(McSlotObjectWrapper::build<Func, Checker>(nullptr, std::move(callback)));
    }

    template<typename Object, typename Func, typename Checker = McSlotObjectWrapper::DummyChecker>
    static McCppAsyncCallbackPtr build(const Object *receiver, Func callback) noexcept
    {
        return McCppAsyncCallbackPtr(
            new McCppAsyncCallback(receiver, callback, static_cast<Checker *>(nullptr)), CallbackDeleter());
    }
    template<typename Func, typename Checker = McSlotObjectWrapper::DummyChecker>
    static McCppAsyncCallbackPtr build(Func callback) noexcept
    {
        return McCppAsyncCallbackPtr(
            new McCppAsyncCallback(std::move(callback), static_cast<Checker *>(nullptr)), CallbackDeleter());
    }

private:
    std::optional<McSlotObjectWrapper> m_callback;
};

#ifdef MC_USE_QT5
Q_DECLARE_METATYPE(McCppAsyncCallback)
#endif
MC_DECL_METATYPE(McCppAsyncCallback)
