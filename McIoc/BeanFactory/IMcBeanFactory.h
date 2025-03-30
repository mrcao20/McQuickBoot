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

#include "../McIocGlobal.h"

QT_BEGIN_NAMESPACE
class QThread;
QT_END_NAMESPACE

class IMcBeanFactory
{
    MC_DEFINE_INTERFACE(IMcBeanFactory)
public:
    template<typename T>
    T getBean(const QString &name, QThread *thread = nullptr) noexcept
    {
        QVariant var = getBean(name, thread);
        return McPrivate::toRealValue<T>(var);
    }

    virtual QVariant getBean(const QString &name, QThread *thread = nullptr) noexcept = 0;
    virtual void moveToThread(const QString &name, QThread *thread) noexcept = 0;

    virtual bool containsBean(const QString &name) const noexcept = 0;
    virtual bool isSingleton(const QString &name) const noexcept = 0;
    virtual bool isPointer(const QString &name) const noexcept = 0;
};

MC_DECL_POINTER(IMcBeanFactory)
