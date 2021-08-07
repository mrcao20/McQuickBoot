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

#include "../IMcConfigurableBeanFactory.h"
#include "../IMcBeanReferenceResolver.h"

MC_FORWARD_DECL_PRIVATE_DATA(McAbstractBeanFactory);

class MCIOC_EXPORT McAbstractBeanFactory : public QObject,
                                           public IMcConfigurableBeanFactory,
                                           public IMcBeanReferenceResolver
{
    Q_OBJECT
public:
    using IMcBeanFactory::getBean;
    using IMcBeanFactory::getBeanPointer;

    explicit McAbstractBeanFactory(QObject *parent = nullptr);
    ~McAbstractBeanFactory() override;

    /*!
     * \brief getBean
     * 
     * 根据bean的名称获取bean，如果没有，则返回空。
     * 如果有，则从bean定义对象获取bean实例。
     * \param name beanName
     * \param thread 将要生存的目标线程
     * \return 返回获取到的对象
     */
    QObjectPtr getBean(const QString &name, QThread *thread = nullptr) noexcept override;
    QObject *getBeanPointer(const QString &name, QThread *thread = nullptr) noexcept override;
    /*!
     * \brief getBeanToVariant
     * 
     * 根据bean的名称获取包含bean实例的QVariant，如果没有，则返回无效值。
     * 如果有，则从bean定义对象获取包含bean实例的QVariant。
     * \param name beanName
     * \param thread 将要生存的目标线程
     * \return 
     */
    QVariant getBeanToVariant(const QString &name, QThread *thread = nullptr) noexcept override;
    /*!
     * \brief containsBean
     * 
     * 检测容器中是否存在该bean。
     * \param name beanName
     * \return 是否存在
     * \retval true 存在
     * \retval false 不存在
     */
    bool containsBean(const QString &name) const noexcept override;
    /*!
     * \brief isSingleton
     * 
     * 检查bean是否为单例
     * \param name
     * \return 
     */
    bool isSingleton(const QString &name) noexcept override;
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
    /*!
     * \brief resolveBeanReference
     * 
     * 根据beanReference生成实例对象。
     * \see IMcBeanReferenceResolver
     * \param beanRef
     * \return 
     */
    QObjectPtr resolveBeanReference(McBeanReferenceConstPtrRef beanRef) noexcept override;
    QObject *resolveBeanReferencePointer(McBeanReferenceConstPtrRef beanRef) noexcept override;
    /*!
     * \brief resolveBeanReferenceToQVariant
     * 
     * 根据beanReference生成包含实例对象的QVariant。
     * \see IMcBeanReferenceResolver
     * \param beanRef
     * \return 
     */
    QVariant resolveBeanReferenceToQVariant(McBeanReferenceConstPtrRef beanRef) noexcept override;
    
protected:
    /*!
     * \brief doCreate
     * 
     * 根据传入的bean定义和目标线程创建一个bean
     * \param beanDefinition bean定义
     * \param thread 目标生存线程
     * \return 创建的bean的包裹QVariant
     * \note 此函数会返回无效值
     */
    virtual QVariant doCreate(IMcBeanDefinitionConstPtrRef beanDefinition, QThread *thread) noexcept = 0;

private:
    MC_DECL_PRIVATE(McAbstractBeanFactory)
};

MC_DECL_POINTER(McAbstractBeanFactory)
