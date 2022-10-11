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
#include "McResolverBeanFactory.h"

#include "BeanBuilder/IMcBeanBuilder.h"
#include "BeanBuilder/Impl/McBeanReference.h"

MC_DECL_PRIVATE_DATA(McResolverBeanFactory)
QThread *targetThread{nullptr}; //!< 生成对象的目标线程
MC_DECL_PRIVATE_DATA_END

McResolverBeanFactory::McResolverBeanFactory() noexcept
{
    MC_NEW_PRIVATE_DATA(McResolverBeanFactory);
}

McResolverBeanFactory::~McResolverBeanFactory() {}

QVariant McResolverBeanFactory::getBean(const QString &name, QThread *thread) noexcept
{
    d->targetThread = thread;
    return super::getBean(name, thread);
}

bool McResolverBeanFactory::registerBeanBuilder(const QString &name, const IMcBeanBuilderPtr &beanBuilder) noexcept
{
    if (beanBuilder.isNull()) {
        return false;
    }
    beanBuilder->setReferenceResolver(this);
    return super::registerBeanBuilder(name, beanBuilder);
}

QVariant McResolverBeanFactory::resolveBeanReference(const McBeanReferencePtr &beanRef) noexcept
{
    if (beanRef.isNull()) {
        return QVariant();
    }
    return getBean(beanRef->name(), d->targetThread);
}

void McResolverBeanFactory::beanReferenceMoveToThread(const McBeanReferencePtr &beanRef, QThread *thread) noexcept
{
    if (beanRef.isNull()) {
        return;
    }
    moveToThread(beanRef->name(), thread);
}

void McResolverBeanFactory::afterBuildBean(const QVariant &bean) noexcept
{
    Q_UNUSED(bean)
}
