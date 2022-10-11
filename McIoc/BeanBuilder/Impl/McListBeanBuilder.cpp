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
#include "McListBeanBuilder.h"

MC_DECL_PRIVATE_DATA(McListBeanBuilder)
QVariantList values;
MC_DECL_PRIVATE_DATA_END

McListBeanBuilder::McListBeanBuilder() noexcept
{
    MC_NEW_PRIVATE_DATA(McListBeanBuilder);
}

McListBeanBuilder::~McListBeanBuilder() {}

void McListBeanBuilder::addValue(const QVariant &var) noexcept
{
    d->values.append(var);
}

void McListBeanBuilder::setValues(const QVariantList &vars) noexcept
{
    d->values = vars;
}

bool McListBeanBuilder::isPointer() const noexcept
{
    return false;
}

QVariant McListBeanBuilder::create() noexcept
{
    auto bean = QVariant(QVariantList());
    return bean;
}

void McListBeanBuilder::complete(QVariant &bean, QThread *thread) noexcept
{
    Q_UNUSED(thread)
    bean.setValue(convert(QVariant(d->values), QVariant()));
}

void McListBeanBuilder::doMoveToThread(const QVariant &bean, QThread *thread, const QVariantHash &properties) noexcept
{
    Q_UNUSED(bean)
    Q_UNUSED(thread)
    Q_UNUSED(properties)
}
