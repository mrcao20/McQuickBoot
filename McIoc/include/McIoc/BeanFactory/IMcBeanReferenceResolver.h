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

#include "../McGlobal.h"

MC_FORWARD_DECL_CLASS(McBeanReference)

class IMcBeanReferenceResolver 
{
public:
    virtual ~IMcBeanReferenceResolver() = default;
    
    /*!
     * \brief resolveBeanReference
     * 
     * 根据beanReference生成实例对象。
     * \param beanRef 被引用对象的信息
     * \return 根据相关信息生成的对象
     */
    virtual QObjectPtr resolveBeanReference(McBeanReferenceConstPtrRef beanRef) noexcept = 0;
    virtual QObject *resolveBeanReferencePointer(McBeanReferenceConstPtrRef beanRef) noexcept = 0;

    virtual bool canResolve(McBeanReferenceConstPtrRef beanRef) noexcept = 0;
    /*!
     * \brief resolveBeanReferenceToQVariant
     * 
     * 根据beanReference生成包含实例对象的QVariant。
     * \param beanRef 被引用对象的信息
     * \return 包含根据相关信息生成的对象的QVariant。
     */
    virtual QVariant resolveBeanReferenceToQVariant(McBeanReferenceConstPtrRef beanRef) noexcept = 0;
};

MC_DECL_POINTER(IMcBeanReferenceResolver)
