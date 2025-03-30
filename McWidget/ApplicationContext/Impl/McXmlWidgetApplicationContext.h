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

#include <McIoc/ApplicationContext/Impl/McReadableApplicationContext.h>

#include "../../McWidgetGlobal.h"

QT_BEGIN_NAMESPACE
MC_FORWARD_DECL_CLASS(QIODevice);
QT_END_NAMESPACE

class MC_WIDGET_EXPORT McXmlWidgetApplicationContext : public McReadableApplicationContext
{
public:
    McXmlWidgetApplicationContext() noexcept;
    explicit McXmlWidgetApplicationContext(const QIODevicePtr &device, const QString &flag = QString()) noexcept;
    explicit McXmlWidgetApplicationContext(const QList<QIODevicePtr> &devices, const QString &flag = QString()) noexcept;
    explicit McXmlWidgetApplicationContext(const IMcBeanBuilderReaderPtr &reader) noexcept;
    McXmlWidgetApplicationContext(
        const IMcConfigurableBeanFactoryPtr &factory, const IMcBeanBuilderReaderPtr &reader) noexcept;

    void setDevice(const QIODevicePtr &device, const QString &flag = QString()) noexcept;
    void setDevices(const QList<QIODevicePtr> &devices, const QString &flag = QString()) noexcept;
};

MC_DECL_POINTER(McXmlWidgetApplicationContext)
