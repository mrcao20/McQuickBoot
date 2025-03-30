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

#include "../IMcReadableApplicationContext.h"
#include "McAbstractApplicationContext.h"

MC_FORWARD_DECL_PRIVATE_DATA(McReadableApplicationContext);

class MC_IOC_EXPORT McReadableApplicationContext
    : public McAbstractApplicationContext
    , public IMcReadableApplicationContext
{
public:
    McReadableApplicationContext() noexcept;
    explicit McReadableApplicationContext(const IMcConfigurableBeanFactoryPtr &factory) noexcept;
    explicit McReadableApplicationContext(const IMcBeanBuilderReaderPtr &reader) noexcept;
    McReadableApplicationContext(
        const IMcConfigurableBeanFactoryPtr &factory, const IMcBeanBuilderReaderPtr &reader) noexcept;
    ~McReadableApplicationContext() override;

    void readBeans() noexcept override;

    void setReader(const IMcBeanBuilderReaderPtr &reader) noexcept override;

private:
    MC_DECL_PRIVATE(McReadableApplicationContext)
};

MC_DECL_POINTER(McReadableApplicationContext)
