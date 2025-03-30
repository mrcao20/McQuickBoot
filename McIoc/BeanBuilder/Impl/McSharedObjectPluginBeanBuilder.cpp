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
#include "McSharedObjectPluginBeanBuilder.h"

bool McSharedObjectPluginBeanBuilder::isPointer() const noexcept
{
    return false;
}

QVariant McSharedObjectPluginBeanBuilder::create() noexcept
{
    auto bean = super::create();
    if (!bean.isValid()) {
        return QVariant();
    }
    return metaType().createSharedPointer(*reinterpret_cast<void **>(const_cast<void *>(bean.constData())));
}
