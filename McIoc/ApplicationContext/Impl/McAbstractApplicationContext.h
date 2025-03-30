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
#include "../IMcApplicationContext.h"

MC_FORWARD_DECL_CLASS(IMcConfigurableBeanFactory)

MC_FORWARD_DECL_PRIVATE_DATA(McAbstractApplicationContext)

class MC_IOC_EXPORT McAbstractApplicationContext
    : public IMcApplicationContext
    , public IMcBeanReferenceResolver
{
public:
    using IMcBeanFactory::getBean;
    using IMcBeanReferenceResolver::resolveBeanReference;

    explicit McAbstractApplicationContext(const IMcConfigurableBeanFactoryPtr &factory) noexcept;
    ~McAbstractApplicationContext() override;

    QVariant getBean(const QString &name, QThread *thread = nullptr) noexcept override;
    void moveToThread(const QString &name, QThread *thread) noexcept override;
    bool containsBean(const QString &name) const noexcept override;
    bool isSingleton(const QString &name) const noexcept override;
    bool isPointer(const QString &name) const noexcept override;

    bool registerBeanBuilder(const QString &name, const IMcBeanBuilderPtr &beanBuilder) noexcept override;
    bool registerBeanBuilder(const QHash<QString, IMcBeanBuilderPtr> &vals) noexcept override;
    IMcBeanBuilderPtr unregisterBeanBuilder(const QString &name) noexcept override;
    bool isContained(const QString &name) const noexcept override;
    QHash<QString, IMcBeanBuilderPtr> getBeanBuilders() const noexcept override;

    void refresh(QThread *thread = nullptr) noexcept override;

    QVariant resolveBeanReference(const McBeanReferencePtr &beanRef) noexcept override;
    void beanReferenceMoveToThread(const McBeanReferencePtr &beanRef, QThread *thread) noexcept override;

private:
    MC_DECL_PRIVATE(McAbstractApplicationContext)
};

MC_DECL_POINTER(McAbstractApplicationContext)
