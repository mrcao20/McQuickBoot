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

#include <QMap>

#include "../McIocGlobal.h"

MC_FORWARD_DECL_CLASS(IMcBeanBuilder)

class IMcBeanBuilderRegistry
{
    MC_DEFINE_INTERFACE(IMcBeanBuilderRegistry)
public:
    virtual bool registerBeanBuilder(const QString &name, const IMcBeanBuilderPtr &beanBuilder) noexcept = 0;
    virtual bool registerBeanBuilder(const QHash<QString, IMcBeanBuilderPtr> &vals) noexcept = 0;
    virtual IMcBeanBuilderPtr unregisterBeanBuilder(const QString &name) noexcept = 0;

    virtual bool isContained(const QString &name) const noexcept = 0;
    virtual QHash<QString, IMcBeanBuilderPtr> getBeanBuilders() const noexcept = 0;
};

MC_DECL_POINTER(IMcBeanBuilderRegistry)
