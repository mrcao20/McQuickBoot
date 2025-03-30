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
#include "McXmlWidgetApplicationContext.h"

#include "BeanBuilderReader/Impl/McXmlWidgetBeanBuilderReader.h"

McXmlWidgetApplicationContext::McXmlWidgetApplicationContext() noexcept {}

McXmlWidgetApplicationContext::McXmlWidgetApplicationContext(const QIODevicePtr &device, const QString &flag) noexcept
    : McXmlWidgetApplicationContext(QList<QIODevicePtr>() << device, flag)
{
}

McXmlWidgetApplicationContext::McXmlWidgetApplicationContext(
    const QList<QIODevicePtr> &devices, const QString &flag) noexcept
{
    setDevices(devices, flag);
}

McXmlWidgetApplicationContext::McXmlWidgetApplicationContext(const IMcBeanBuilderReaderPtr &reader) noexcept
    : McReadableApplicationContext(reader)
{
}

McXmlWidgetApplicationContext::McXmlWidgetApplicationContext(
    const IMcConfigurableBeanFactoryPtr &factory, const IMcBeanBuilderReaderPtr &reader) noexcept
    : McReadableApplicationContext(factory, reader)
{
}

void McXmlWidgetApplicationContext::setDevice(const QIODevicePtr &device, const QString &flag) noexcept
{
    setDevices(QList<QIODevicePtr>() << device, flag);
}

void McXmlWidgetApplicationContext::setDevices(const QList<QIODevicePtr> &devices, const QString &flag) noexcept
{
    setReader(McXmlWidgetBeanBuilderReaderPtr::create(devices, flag));
}
