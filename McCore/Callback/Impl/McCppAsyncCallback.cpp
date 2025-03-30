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
#include "McCppAsyncCallback.h"

MC_INIT(McCppAsyncCallback)
mcRegisterMetaTypeSimple<McCppAsyncCallback>();
MC_INIT_END

McCppAsyncCallback::McCppAsyncCallback() noexcept {}

McCppAsyncCallback::~McCppAsyncCallback() {}

McCppAsyncCallback::McCppAsyncCallback(const McCppAsyncCallback &o) noexcept
    : m_callback(o.m_callback)
{
}

McCppAsyncCallback &McCppAsyncCallback::operator=(const McCppAsyncCallback &o) noexcept
{
    McCppAsyncCallback copy(o);
    std::swap(m_callback, copy.m_callback);
    return *this;
}

McCppAsyncCallback::McCppAsyncCallback(McCppAsyncCallback &&o) noexcept
    : m_callback(o.m_callback)
{
    o.m_callback.reset();
}

McCppAsyncCallback &McCppAsyncCallback::operator=(McCppAsyncCallback &&o) noexcept
{
    McCppAsyncCallback copy(std::move(o));
    std::swap(m_callback, copy.m_callback);
    return *this;
}

QVariant McCppAsyncCallback::call(const QVariantList &varList) const noexcept
{
    if (!m_callback.has_value()) {
        return QVariant();
    }
    return m_callback->call(varList);
}
