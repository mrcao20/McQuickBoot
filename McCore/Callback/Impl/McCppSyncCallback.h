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

class MC_CORE_EXPORT McCppSyncCallback : public McAbstractSyncCallback
{
    Q_OBJECT
    MC_DECL_INIT(McCppSyncCallback)
    MC_INTERFACES(McAbstractSyncCallback)
    MC_DECL_POINTER(McCppSyncCallback)
public:
    McCppSyncCallback(const McCppSyncCallback &o) noexcept;
    McCppSyncCallback &operator=(const McCppSyncCallback &o) noexcept;
    McCppSyncCallback(McCppSyncCallback &&o) noexcept;
    McCppSyncCallback &operator=(McCppSyncCallback &&o) noexcept;

    template<typename Func>
    McCppSyncCallback(const typename QtPrivate::FunctionPointer<Func>::Object *receiver, Func callback) noexcept
    {
        m_callback = std::make_optional(McSlotObjectWrapper::build(receiver, callback));
    }

    template<typename Func>
    McCppSyncCallback(Func callback) noexcept
    {
        m_callback = std::make_optional(McSlotObjectWrapper::build(nullptr, std::move(callback)));
    }

    template<typename Func>
    static McCppSyncCallbackPtr build(
        const typename QtPrivate::FunctionPointer<Func>::Object *receiver, Func callback) noexcept
    {
        return McCppSyncCallbackPtr(new McCppSyncCallback(receiver, callback), CallbackDeleter());
    }
    template<typename Func>
    static McCppSyncCallbackPtr build(Func callback) noexcept
    {
        return McCppSyncCallbackPtr(new McCppSyncCallback(std::move(callback)), CallbackDeleter());
    }

protected:
    void syncCall(const QVariantList &varList) const noexcept override;

private:
    std::optional<McSlotObjectWrapper> m_callback;
};

MC_DECL_METATYPE(McCppSyncCallback)
