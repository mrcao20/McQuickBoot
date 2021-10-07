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

#include <QMap>

MC_FORWARD_DECL_CLASS(IMcBeanDefinition)

class IMcBeanDefinitionRegistry 
{
public:
    virtual ~IMcBeanDefinitionRegistry() = default;
    
    /*!
     * \brief registerBeanDefinition
     * 
     * 将bean的定义注册到容器中, 如果已经存在则替换
     * \param name beanName
     * \param beanDefinition bean的定义，包含bean中所有相关数据
     */
    virtual bool registerBeanDefinition(const QString &name,
                                        IMcBeanDefinitionConstPtrRef beanDefinition) noexcept = 0;
    virtual bool registerBeanDefinition(
        const QHash<QString, IMcBeanDefinitionPtr> &vals) noexcept = 0;

    /*!
     * \brief unregisterBeanDefinition
     * 
     * 将beanDefinition从容器中移除
     * \param name beanName
     * \return 被移除的beanDefinition，如果不存在则返回空
     */
    virtual IMcBeanDefinitionPtr unregisterBeanDefinition(const QString &name) noexcept = 0;

    virtual bool canRegister(IMcBeanDefinitionConstPtrRef beanDefinition) const noexcept = 0;

    /*!
     * \brief isContained
     * 
     * 检测该bean是否已经存在
     * \param name beanName
     * \return 是否存在
     * \retval true 存在
     * \retval false 不存在
     */
    virtual bool isContained(const QString &name) const noexcept = 0;

    /*!
     * \brief getBeanDefinitions
     * 
     * 获取所有bean定义
     * \return 返回bean的定义的集合。key为beanName，value为bean定义
     */
    virtual QHash<QString, IMcBeanDefinitionPtr> getBeanDefinitions() const noexcept = 0;
};

MC_DECL_POINTER(IMcBeanDefinitionRegistry)
