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

#include <McIoc/BeanBuilder/Impl/McObjectClassBeanBuilder.h>

#include "../../McWidgetGlobal.h"

class MC_WIDGET_EXPORT McWidgetBeanBuilder : public McObjectClassBeanBuilder
{
    MC_DECL_SUPER(McObjectClassBeanBuilder)
protected:
    void complete(QVariant &bean, QThread *thread) noexcept override;
    void setParent(QObject *bean, QObject *parent) noexcept override;
    QVariant convertRef(const QVariant &value, const QVariant &extra) const noexcept override;
};

MC_DECL_POINTER(McWidgetBeanBuilder)
