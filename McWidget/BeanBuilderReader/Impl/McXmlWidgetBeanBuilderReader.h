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

#include <McIoc/BeanBuilderReader/Impl/McXmlBeanBuilderReader.h>

#include "../../McWidgetGlobal.h"

class MC_WIDGET_EXPORT McXmlWidgetBeanBuilderReader : public McXmlBeanBuilderReader
{
    MC_DECL_SUPER(McXmlBeanBuilderReader)
public:
    McXmlWidgetBeanBuilderReader(const QIODevicePtr &device, const QString &flag = QString()) noexcept;
    McXmlWidgetBeanBuilderReader(const QList<QIODevicePtr> &devices, const QString &flag = QString()) noexcept;

protected:
    McObjectClassBeanBuilderPtr createObjectBeanBuilder(McMetaType metaType, bool isPointer) const noexcept override;
};

MC_DECL_POINTER(McXmlWidgetBeanBuilderReader)
