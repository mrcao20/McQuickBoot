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

#include "../IMcConfigurableWidgetBeanFactory.h"

MC_FORWARD_DECL_PRIVATE_DATA(McAbstractWidgetBeanFactory);

class MCWIDGETIOC_EXPORT McAbstractWidgetBeanFactory : public QObject,
                                                       public IMcConfigurableWidgetBeanFactory
{
    Q_OBJECT
public:
    using IMcWidgetBeanFactory::getBean;

    explicit McAbstractWidgetBeanFactory(QObject *parent = nullptr);
    ~McAbstractWidgetBeanFactory() override;

    QWidget *getBean(const QString &name, QWidget *parent = nullptr) const noexcept override;

    bool containsBean(const QString &name) const noexcept override;
    /*!
     * \brief registerBeanDefinition
     * 
     * 向容器中注册bean定义。
     * \param name beanName
     * \param beanDefinition bean的定义
     */
    bool registerBeanDefinition(const QString &name,
                                IMcBeanDefinitionConstPtrRef beanDefinition) noexcept override;

    IMcBeanDefinitionPtr unregisterBeanDefinition(const QString &name) noexcept override;

    /*!
     * \brief isContained
     * 
     * 是否已经注册过该bean
     * \param name beanName
     * \return 是否已经注册过
     * \retval true 已经注册过
     * \retval false 没有被注册
     */
    bool isContained(const QString &name) const noexcept override;

    bool canRegister(IMcBeanDefinitionConstPtrRef beanDefinition) const noexcept override;
    /*!
     * \brief getBeanDefinitions
     * 
     * 获取所有bean定义的集合
     * \return bean定义的集合。
     * \see IMcBeanDefinitionRegistry
     */
    QHash<QString, IMcBeanDefinitionPtr> getBeanDefinitions() const noexcept override;

protected:
    virtual QWidget *doCreate(IMcBeanDefinitionConstPtrRef beanDefinition) const noexcept = 0;

private:
    MC_DECL_PRIVATE(McAbstractWidgetBeanFactory)
};

MC_DECL_POINTER(McAbstractWidgetBeanFactory)
