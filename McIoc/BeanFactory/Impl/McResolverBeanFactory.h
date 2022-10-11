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

#include "../../BeanBuilder/IMcBeanReferenceResolver.h"
#include "McAbstractBeanFactory.h"

MC_FORWARD_DECL_PRIVATE_DATA(McResolverBeanFactory)

class MC_IOC_EXPORT McResolverBeanFactory
    : public McAbstractBeanFactory
    , public IMcBeanReferenceResolver
{
    MC_DECL_SUPER(McAbstractBeanFactory)
public:
    using IMcBeanFactory::getBean;
    using IMcBeanReferenceResolver::resolveBeanReference;

    McResolverBeanFactory() noexcept;
    ~McResolverBeanFactory();

    QVariant getBean(const QString &name, QThread *thread = nullptr) noexcept override;

    bool registerBeanBuilder(const QString &name, const IMcBeanBuilderPtr &beanBuilder) noexcept override;

    QVariant resolveBeanReference(const McBeanReferencePtr &beanRef) noexcept override;
    void beanReferenceMoveToThread(const McBeanReferencePtr &beanRef, QThread *thread) noexcept override;

protected:
    void afterBuildBean(const QVariant &bean) noexcept override;

private:
    MC_DECL_PRIVATE(McResolverBeanFactory)
};

MC_DECL_POINTER(McResolverBeanFactory)
