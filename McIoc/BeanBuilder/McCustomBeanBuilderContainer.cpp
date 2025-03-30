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
#include "McCustomBeanBuilderContainer.h"

using BuilderFactoryType = QHash<QString, IMcCustomBeanBuilderFactoryPtr>;
Q_GLOBAL_STATIC(BuilderFactoryType, staticBeanBuilderFactories)

void McCustomBeanBuilderContainer::addBuilderFactory(
    const QString &className, const IMcCustomBeanBuilderFactoryPtr &factory) noexcept
{
    staticBeanBuilderFactories->insert(className, factory);
}

McAbstractBeanBuilderPtr McCustomBeanBuilderContainer::getBuilder(const QString &className) noexcept
{
    auto factory = staticBeanBuilderFactories->value(className);
    if (factory.isNull()) {
        return McAbstractBeanBuilderPtr();
    }
    return factory->create();
}

bool McCustomBeanBuilderContainer::containsBuilder(const QString &className) noexcept
{
    return staticBeanBuilderFactories->contains(className);
}
