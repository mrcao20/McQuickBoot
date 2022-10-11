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
#include "McReadableApplicationContext.h"

#include "BeanBuilderReader/IMcBeanBuilderReader.h"
#include "BeanFactory/Impl/McDefaultBeanFactory.h"

MC_DECL_PRIVATE_DATA(McReadableApplicationContext)
IMcBeanBuilderReaderPtr beanBuilderReader;
MC_DECL_PRIVATE_DATA_END

McReadableApplicationContext::McReadableApplicationContext() noexcept
    : McReadableApplicationContext(McDefaultBeanFactoryPtr::create())
{
}

McReadableApplicationContext::McReadableApplicationContext(const IMcConfigurableBeanFactoryPtr &factory) noexcept
    : McAbstractApplicationContext(factory)
{
    MC_NEW_PRIVATE_DATA(McReadableApplicationContext);
}

McReadableApplicationContext::McReadableApplicationContext(const IMcBeanBuilderReaderPtr &reader) noexcept
    : McReadableApplicationContext(McDefaultBeanFactoryPtr::create(), reader)
{
}

McReadableApplicationContext::McReadableApplicationContext(
    const IMcConfigurableBeanFactoryPtr &factory, const IMcBeanBuilderReaderPtr &reader) noexcept
    : McReadableApplicationContext(factory)
{
    setReader(reader);
}

McReadableApplicationContext::~McReadableApplicationContext() {}

void McReadableApplicationContext::readBeans() noexcept
{
    d->beanBuilderReader->readBeanBuilder(this);
}

void McReadableApplicationContext::setReader(const IMcBeanBuilderReaderPtr &reader) noexcept
{
    d->beanBuilderReader = reader;

    readBeans();
}
