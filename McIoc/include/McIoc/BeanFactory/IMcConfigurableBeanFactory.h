#pragma once

#include "IMcBeanFactory.h"
#include "IMcBeanDefinitionRegistry.h"

class IMcConfigurableBeanFactory : public IMcBeanFactory, public IMcBeanDefinitionRegistry
{
public:
    virtual ~IMcConfigurableBeanFactory() = default;
};

MC_DECL_POINTER(IMcConfigurableBeanFactory)
