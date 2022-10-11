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

#include "../IMcCustomBeanBuilderFactory.h"

#include "McCustomBeanBuilder.h"

template<typename T, typename BUILDER>
class McCustomBeanBuilderFactory : public IMcCustomBeanBuilderFactory
{
    using Type = T;
    using BuildFuncType = typename McCustomBeanBuilder<Type>::BuildFuncType;

public:
    McCustomBeanBuilderFactory(const BuildFuncType &func, bool isShared = false)
        : m_func(func)
        , m_isShared(isShared)
    {
    }

    McAbstractBeanBuilderPtr create() noexcept override { return QSharedPointer<BUILDER>::create(m_func, m_isShared); }

private:
    bool m_isShared{false};
    BuildFuncType m_func;
};

template<typename T, typename BUILDER>
using McCustomBeanBuilderFactoryPtr = QSharedPointer<McCustomBeanBuilderFactory<T, BUILDER>>;
