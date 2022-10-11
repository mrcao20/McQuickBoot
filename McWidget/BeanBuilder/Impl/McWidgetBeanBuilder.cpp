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
#include "McWidgetBeanBuilder.h"

#include <QWidget>

void McWidgetBeanBuilder::complete(QVariant &bean, QThread *thread) noexcept
{
    //! Widget控件不能移动线程
    Q_UNUSED(thread)
    super::complete(bean, nullptr);
}

void McWidgetBeanBuilder::setParent(QObject *bean, QObject *parent) noexcept
{
    auto widgetBean = qobject_cast<QWidget *>(bean);
    auto widgetParent = qobject_cast<QWidget *>(parent);
    if (widgetBean == nullptr || widgetParent == nullptr) {
        super::setParent(bean, parent);
    } else {
        widgetBean->setParent(widgetParent);
    }
}

QVariant McWidgetBeanBuilder::convertRef(const QVariant &value, const QVariant &extra) const noexcept
{
    auto target = super::convertRef(value, extra);
    auto targetWidget = target.value<QWidget *>();
    if (targetWidget == nullptr || targetWidget->parentWidget() != nullptr) {
        return target;
    }
    auto parentWidget = extra.value<QWidget *>();
    if (parentWidget == nullptr) {
        return target;
    }
    targetWidget->setParent(parentWidget);
    return target;
}
