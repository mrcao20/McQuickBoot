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

class IMcBeanBuildable;

class MC_IOC_EXPORT McGadgetBeanBuilder : public McAbstractBeanBuilder
{
public:
    void setClassName(const QByteArray &className) noexcept;

    bool isPointer() const noexcept override;

protected:
    void complete(QVariant &bean, QThread *thread) noexcept override;
    void doMoveToThread(const QVariant &bean, QThread *thread, const QVariantHash &properties) noexcept override;

private:
    void addPropertyValue(void *bean, const QMetaObject *metaObject, const QVariantMap &pros);

    void callStartFunction(void *bean, const QMetaObject *metaObject, IMcBeanBuildable *buildableBean) noexcept;
    void callFinishedFunction(void *bean, const QMetaObject *metaObject, IMcBeanBuildable *buildableBean) noexcept;
    void callTagFunction(void *bean, const QMetaObject *metaObject, const char *tag) noexcept;
};

MC_DECL_POINTER(McGadgetBeanBuilder)
