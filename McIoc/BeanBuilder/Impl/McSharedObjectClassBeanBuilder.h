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

#include "McObjectClassBeanBuilder.h"

class MC_IOC_EXPORT McSharedObjectClassBeanBuilder : public McObjectClassBeanBuilder
{
    MC_DECL_SUPER(McObjectClassBeanBuilder)
public:
    bool isPointer() const noexcept override;

protected:
    QVariant create() noexcept override;
};

MC_DECL_POINTER(McSharedObjectClassBeanBuilder)
