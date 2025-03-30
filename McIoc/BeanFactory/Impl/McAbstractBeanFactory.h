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

#include "../IMcConfigurableBeanFactory.h"

MC_FORWARD_DECL_PRIVATE_DATA(McAbstractBeanFactory)

class MC_IOC_EXPORT McAbstractBeanFactory : public IMcConfigurableBeanFactory
{
public:
    using IMcBeanFactory::getBean;

    McAbstractBeanFactory() noexcept;
    ~McAbstractBeanFactory() override;

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

protected:
    virtual void afterBuildBean(const QVariant &bean) noexcept = 0;

private:
    MC_DECL_PRIVATE(McAbstractBeanFactory)
};

MC_DECL_POINTER(McAbstractBeanFactory)
