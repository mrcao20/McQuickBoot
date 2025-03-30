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

#include "Impl/McCustomBeanBuilderFactory.h"

class MC_IOC_EXPORT McCustomBeanBuilderContainer
{
public:
    static void addBuilderFactory(const QString &className, const IMcCustomBeanBuilderFactoryPtr &factory) noexcept;
    static McAbstractBeanBuilderPtr getBuilder(const QString &className) noexcept;
    static bool containsBuilder(const QString &className) noexcept;
};

template<typename T, typename BUILDER = McCustomBeanBuilder<T>>
void mcAddCustomBuilderFactory(
    const typename McCustomBeanBuilder<T>::BuildFuncType &func = nullptr, bool isShared = false)
{
#ifdef MC_USE_QT5
    const char *className = McPrivate::typenameHelper<T>().data();
#else
    constexpr QMetaType qmetaType = QMetaType::fromType<T>();
    constexpr const char *className = qmetaType.name();
#endif
    McCustomBeanBuilderContainer::addBuilderFactory(
        QLatin1String(className), McCustomBeanBuilderFactoryPtr<T, BUILDER>::create(func, isShared));
}
