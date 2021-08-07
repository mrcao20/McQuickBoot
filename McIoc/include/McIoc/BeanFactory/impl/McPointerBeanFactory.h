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

#include "McAbstractNormalBeanFactory.h"

MC_FORWARD_DECL_PRIVATE_DATA(McPointerBeanFactory);

class MCIOC_EXPORT McPointerBeanFactory : public McAbstractNormalBeanFactory
{
    Q_OBJECT
public:
    McPointerBeanFactory(QObject *parent = nullptr) noexcept;
    ~McPointerBeanFactory() override;

    bool canRegister(IMcBeanDefinitionConstPtrRef beanDefinition) const noexcept override;
    bool canResolve(McBeanReferenceConstPtrRef beanRef) noexcept override;

protected:
    QVariant convertToQVariant(QObject *obj) noexcept override;
    QVariant convertToQVariant(void *gadget, const QMetaObject *metaObj) noexcept override;

private:
    MC_DECL_PRIVATE(McPointerBeanFactory)
};

MC_DECL_POINTER(McPointerBeanFactory)
