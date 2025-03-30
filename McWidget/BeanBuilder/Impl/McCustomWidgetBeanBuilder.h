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

#include <McIoc/BeanBuilder/Impl/McCustomBeanBuilder.h>

template<typename T>
class McCustomWidgetBeanBuilder : public McCustomBeanBuilder<T>
{
    MC_DECL_SUPER(McCustomBeanBuilder<T>)

public:
    McCustomWidgetBeanBuilder(const typename super::BuildFuncType &func, bool)
        : super(func, false)
    {
    }

protected:
    void complete(QVariant &bean, QThread *thread) noexcept override
    {
        Q_UNUSED(thread)
        super::complete(bean, nullptr);
    }

    QVariant convertRef(const QVariant &value, const QVariant &extra) const noexcept override
    {
        auto target = super::convertRef(value, extra);
        auto targetWidget = target.template value<QWidget *>();
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
};

template<typename T>
using McCustomWidgetBeanBuilderPtr = QSharedPointer<McCustomWidgetBeanBuilder<T>>;
