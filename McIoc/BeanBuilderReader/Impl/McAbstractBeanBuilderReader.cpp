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
#include "McAbstractBeanBuilderReader.h"

MC_DECL_PRIVATE_DATA(McAbstractBeanBuilderReader)
IMcBeanBuilderRegistry *registry{nullptr};
MC_DECL_PRIVATE_DATA_END

McAbstractBeanBuilderReader::McAbstractBeanBuilderReader() noexcept
{
    MC_NEW_PRIVATE_DATA(McAbstractBeanBuilderReader);
}

McAbstractBeanBuilderReader::~McAbstractBeanBuilderReader() {}

IMcBeanBuilderRegistry *McAbstractBeanBuilderReader::registry() const noexcept
{
    return d->registry;
}

void McAbstractBeanBuilderReader::readBeanBuilder(IMcBeanBuilderRegistry *registry) noexcept
{
    d->registry = registry;
    if (registry == nullptr) {
        return;
    }
    doReadBeanBuilder();
}
