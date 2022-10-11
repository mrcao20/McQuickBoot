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
#include "McMapBeanBuilder.h"

MC_DECL_PRIVATE_DATA(McMapBeanBuilder)
QMap<QVariant, QVariant> values;
MC_DECL_PRIVATE_DATA_END

McMapBeanBuilder::McMapBeanBuilder() noexcept
{
    MC_NEW_PRIVATE_DATA(McMapBeanBuilder);
}

McMapBeanBuilder::~McMapBeanBuilder() {}

void McMapBeanBuilder::addValue(const QVariant &key, const QVariant &value) noexcept
{
    d->values.insert(key, value);
}

void McMapBeanBuilder::setValues(const QMap<QVariant, QVariant> &values) noexcept
{
    d->values = values;
}

bool McMapBeanBuilder::isPointer() const noexcept
{
    return false;
}

QVariant McMapBeanBuilder::create() noexcept
{
    auto bean = QVariant::fromValue(QMap<QVariant, QVariant>());
    return bean;
}

void McMapBeanBuilder::complete(QVariant &bean, QThread *thread) noexcept
{
    Q_UNUSED(thread)
    bean.setValue(convert(QVariant::fromValue(d->values), QVariant()));
}

void McMapBeanBuilder::doMoveToThread(const QVariant &bean, QThread *thread, const QVariantHash &properties) noexcept
{
    Q_UNUSED(bean)
    Q_UNUSED(thread)
    Q_UNUSED(properties)
}
