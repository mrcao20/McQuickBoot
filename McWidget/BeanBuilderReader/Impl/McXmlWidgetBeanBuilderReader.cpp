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
#include "McXmlWidgetBeanBuilderReader.h"

#include <QWidget>

#include "BeanBuilder/Impl/McWidgetBeanBuilder.h"

McXmlWidgetBeanBuilderReader::McXmlWidgetBeanBuilderReader(const QIODevicePtr &device, const QString &flag) noexcept
    : McXmlWidgetBeanBuilderReader(QList<QIODevicePtr>() << device, flag)
{
}

McXmlWidgetBeanBuilderReader::McXmlWidgetBeanBuilderReader(
    const QList<QIODevicePtr> &devices, const QString &flag) noexcept
    : McXmlBeanBuilderReader(devices, flag)
{
}

McObjectClassBeanBuilderPtr McXmlWidgetBeanBuilderReader::createObjectBeanBuilder(
    McMetaType metaType, bool isPointer) const noexcept
{
#ifdef MC_USE_QT5
    auto metaObj = QMetaType::metaObjectForType(metaType.pMetaType());
#else
    auto metaObj = metaType.pMetaType().metaObject();
#endif
    if (metaObj == nullptr || !metaObj->inherits(&QWidget::staticMetaObject)) {
        return super::createObjectBeanBuilder(metaType, isPointer);
    }
    return McWidgetBeanBuilderPtr::create();
}
