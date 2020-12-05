#pragma once

#include "../McGlobal.h"

class IMcBeanDefinitionRegistry;

class IMcBeanDefinitionReader {
public:
    virtual ~IMcBeanDefinitionReader() = default;
    
    /*!
     * \brief readBeanDefinition
     * 
     * 读取beanDefinition，并将其注册进工厂。
     * \param registry beanDefinition的注册器，将读出的beanDefinition注册进工厂中
     */
    virtual void readBeanDefinition(IMcBeanDefinitionRegistry *registry) noexcept = 0;
};

MC_DECL_POINTER(IMcBeanDefinitionReader)
