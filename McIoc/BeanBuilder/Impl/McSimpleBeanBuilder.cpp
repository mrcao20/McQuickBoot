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
#include "McSimpleBeanBuilder.h"

MC_DECL_PRIVATE_DATA(McSimpleBeanBuilder)
QVariant bean;
bool isPointer{false};
MC_DECL_PRIVATE_DATA_END

McSimpleBeanBuilder::McSimpleBeanBuilder(const QVariant &bean) noexcept
{
    MC_NEW_PRIVATE_DATA(McSimpleBeanBuilder);

    d->bean = bean;
}

McSimpleBeanBuilder::~McSimpleBeanBuilder() {}

void McSimpleBeanBuilder::setPointer(bool val) noexcept
{
    d->isPointer = val;
}

QVariant McSimpleBeanBuilder::build(QThread *thread) noexcept
{
    Q_UNUSED(thread)
    return d->bean;
}

void McSimpleBeanBuilder::moveToThread(QThread *thread) noexcept
{
    Q_UNUSED(thread)
}

bool McSimpleBeanBuilder::isSingleton() const noexcept
{
    return true;
}

bool McSimpleBeanBuilder::isPointer() const noexcept
{
    return d->isPointer;
}

const QMetaObject *McSimpleBeanBuilder::getBeanMetaObject() const noexcept
{
    auto obj = d->bean.value<QObject *>();
    if (obj == nullptr) {
        obj = d->bean.value<QObjectPtr>().data();
    }
    if (obj == nullptr) {
        return nullptr;
    }
    return obj->metaObject();
}

void McSimpleBeanBuilder::setReferenceResolver(IMcBeanReferenceResolver *resolver) noexcept
{
    Q_UNUSED(resolver)
}
