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
    virtual void registerBeanDefinition(const QString &name, IMcBeanDefinitionConstPtrRef beanDefinition) noexcept = 0;
    
    /*!
     * \brief unregisterBeanDefinition
     * 
     * 将beanDefinition从容器中移除
     * \param name beanName
     * \return 被移除的beanDefinition，如果不存在则返回空
     */
    virtual IMcBeanDefinitionPtr unregisterBeanDefinition(const QString &name) noexcept = 0;
    
    /*!
     * \brief isContained
     * 
     * 检测该bean是否已经存在
     * \param name beanName
     * \return 是否存在
     * \retval true 存在
     * \retval false 不存在
     */
    virtual bool isContained(const QString &name) noexcept = 0;
    
    /*!
     * \brief getBeanDefinitions
     * 
     * 获取所有bean定义
     * \return 返回bean的定义的集合。key为beanName，value为bean定义
     */
    virtual QHash<QString, IMcBeanDefinitionPtr> getBeanDefinitions() noexcept = 0;
};

MC_DECL_POINTER(IMcBeanDefinitionRegistry)
