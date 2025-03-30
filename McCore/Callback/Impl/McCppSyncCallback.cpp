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
#include "McCppSyncCallback.h"

MC_INIT(McCppSyncCallback)
mcRegisterMetaTypeSimple<McCppSyncCallback>();
MC_INIT_END

McCppSyncCallback::McCppSyncCallback() noexcept {}

McCppSyncCallback::~McCppSyncCallback() {}

McCppSyncCallback::McCppSyncCallback(const McCppSyncCallback &o) noexcept
    : m_callback(o.m_callback)
{
}

McCppSyncCallback &McCppSyncCallback::operator=(const McCppSyncCallback &o) noexcept
{
    McCppSyncCallback copy(o);
    std::swap(m_callback, copy.m_callback);
    return *this;
}

McCppSyncCallback::McCppSyncCallback(McCppSyncCallback &&o) noexcept
    : m_callback(o.m_callback)
{
    o.m_callback.reset();
}

McCppSyncCallback &McCppSyncCallback::operator=(McCppSyncCallback &&o) noexcept
{
    McCppSyncCallback copy(std::move(o));
    std::swap(m_callback, copy.m_callback);
    return *this;
}

void McCppSyncCallback::syncCall(const QVariantList &varList) const noexcept
{
    if (!m_callback.has_value()) {
        return;
    }
    m_callback->call(varList);
}
