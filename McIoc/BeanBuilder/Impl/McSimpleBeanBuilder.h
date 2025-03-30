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

#include "../IMcBeanBuilder.h"

MC_FORWARD_DECL_PRIVATE_DATA(McSimpleBeanBuilder)

class MC_IOC_EXPORT McSimpleBeanBuilder : public IMcBeanBuilder
{
public:
    McSimpleBeanBuilder(const QVariant &bean) noexcept;
    ~McSimpleBeanBuilder();

    void setPointer(bool val) noexcept;

    QVariant build(QThread *thread) noexcept override;
    void moveToThread(QThread *thread) noexcept override;
    bool isSingleton() const noexcept override;
    bool isPointer() const noexcept override;
    const QMetaObject *getBeanMetaObject() const noexcept override;
    void setReferenceResolver(IMcBeanReferenceResolver *resolver) noexcept override;

private:
    MC_DECL_PRIVATE(McSimpleBeanBuilder)
};

MC_DECL_POINTER(McSimpleBeanBuilder)
