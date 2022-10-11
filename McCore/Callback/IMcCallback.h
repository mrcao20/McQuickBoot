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

class IMcCallback
{
    MC_DEFINE_INTERFACE(IMcCallback)
public:
    struct CallbackDeleter
    {
        void operator()(IMcCallback *ptr) { ptr->destroy(); }
    };

    virtual void destroy() noexcept = 0;
    virtual void call(const QVariantList &varList) const noexcept = 0;

    void call(const QVariant &var) const noexcept { call(QVariantList() << var); }

    template<typename... Args>
    std::enable_if_t<!(sizeof...(Args) == 1
                         && QMetaType::fromType<QtPrivate::List<Args...>::Car>() == QMetaType::fromType<QVariantList>()),
        void>
        call(Args &&...args) const noexcept
    {
        QVariantList vars;
        (vars << ... << McPrivate::toQVariant(std::forward<Args>(args)));
        call(vars);
    }

    void operator()(const QVariantList &varList) const noexcept { call(varList); }

    void operator()(const QVariant &var) const noexcept { call(var); }

    template<typename... Args>
    void operator()(Args &&...args) const noexcept
    {
        call(std::forward<Args>(args)...);
    }
};

MC_DECL_METATYPE(IMcCallback)
