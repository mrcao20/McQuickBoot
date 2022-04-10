/*
 * MIT License
 *
 * Copyright (c) 2021 mrcao20
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
