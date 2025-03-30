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
#include "McAbstractSyncCallback.h"

/*!
 * \brief The McCppSyncCallback class
 * 调用此类的call方法，回调函数并不会立即执行，它将在线程所有权回归到构造此类的线程中时才会执行。
 * 即它会将线程同步到构造此类的线程(通常为主线程，但不必须)之后才会执行回调函数。
 * \note 通常情况下你都应该调用此类的静态函数build来构造，否则可能会得到意料之外的效果。
 */
class MC_CORE_EXPORT McCppSyncCallback : public McAbstractSyncCallback
{
    Q_OBJECT
    MC_DECL_INIT(McCppSyncCallback)
    MC_INTERFACES(McAbstractSyncCallback)
    MC_DECL_POINTER(McCppSyncCallback)
public:
    McCppSyncCallback() noexcept;
    ~McCppSyncCallback() override;

    McCppSyncCallback(const McCppSyncCallback &o) noexcept;
    McCppSyncCallback &operator=(const McCppSyncCallback &o) noexcept;
    McCppSyncCallback(McCppSyncCallback &&o) noexcept;
    McCppSyncCallback &operator=(McCppSyncCallback &&o) noexcept;

    template<typename Object, typename Func, typename Checker = McSlotObjectWrapper::DummyChecker>
    McCppSyncCallback(const Object *receiver, Func callback, Checker *dummy = nullptr) noexcept
    {
        Q_UNUSED(dummy)
        m_callback = std::make_optional(McSlotObjectWrapper::build<Func, Checker>(receiver, callback));
    }

    template<typename Func, typename Checker = McSlotObjectWrapper::DummyChecker>
    McCppSyncCallback(Func callback, Checker *dummy = nullptr) noexcept
    {
        Q_UNUSED(dummy)
        m_callback = std::make_optional(McSlotObjectWrapper::build<Func, Checker>(nullptr, std::move(callback)));
    }

    template<typename Object, typename Func, typename Checker = McSlotObjectWrapper::DummyChecker>
    static McCppSyncCallbackPtr build(const Object *receiver, Func callback) noexcept
    {
        return McCppSyncCallbackPtr(
            new McCppSyncCallback(receiver, callback, static_cast<Checker *>(nullptr)), CallbackDeleter());
    }
    template<typename Func, typename Checker = McSlotObjectWrapper::DummyChecker>
    static McCppSyncCallbackPtr build(Func callback) noexcept
    {
        return McCppSyncCallbackPtr(
            new McCppSyncCallback(std::move(callback), static_cast<Checker *>(nullptr)), CallbackDeleter());
    }

protected:
    void syncCall(const QVariantList &varList) const noexcept override;

private:
    std::optional<McSlotObjectWrapper> m_callback;
};

#ifdef MC_USE_QT5
Q_DECLARE_METATYPE(McCppSyncCallback)
#endif
MC_DECL_METATYPE(McCppSyncCallback)
