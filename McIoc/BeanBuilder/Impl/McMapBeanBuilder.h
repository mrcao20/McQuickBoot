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

#include "McAbstractBeanBuilder.h"

MC_FORWARD_DECL_PRIVATE_DATA(McMapBeanBuilder)

class MC_IOC_EXPORT McMapBeanBuilder : public McAbstractBeanBuilder
{
public:
    McMapBeanBuilder() noexcept;
    ~McMapBeanBuilder();

    void addValue(const QVariant &key, const QVariant &value) noexcept;
    void setValues(const QMap<QVariant, QVariant> &values) noexcept;

    bool isPointer() const noexcept override;

protected:
    QVariant create() noexcept override;
    void complete(QVariant &bean, QThread *thread) noexcept override;
    void doMoveToThread(const QVariant &bean, QThread *thread, const QVariantHash &properties) noexcept override;

private:
    MC_DECL_PRIVATE(McMapBeanBuilder)
};

MC_DECL_POINTER(McMapBeanBuilder)
