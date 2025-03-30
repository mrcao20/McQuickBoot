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
    virtual QVariant call(const QVariantList &varList) const noexcept = 0;

    QVariant call(const QVariant &var) const noexcept { return call(QVariantList() << var); }

    template<typename... Args>
    QVariant call(const Args &...args) const noexcept
    {
        QVariantList vars;
        (vars << ... << McPrivate::toQVariant(args));
        return call(vars);
    }
    template<typename R, typename... Args>
    R call(const Args &...args) const noexcept
    {
        QVariantList vars;
        (vars << ... << McPrivate::toQVariant(args));
        return McPrivate::toRealValue<R>(call(vars));
    }

    QVariant operator()(const QVariantList &varList) const noexcept { return call(varList); }

    QVariant operator()(const QVariant &var) const noexcept { return call(var); }

    template<typename... Args>
    QVariant operator()(Args &&...args) const noexcept
    {
        return call(std::forward<Args>(args)...);
    }
};

MC_DECL_METATYPE(IMcCallback)
