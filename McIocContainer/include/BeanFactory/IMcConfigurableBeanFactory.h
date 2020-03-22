#pragma once

#include "IMcBeanFactory.h"
#include "IMcBeanDefinitionRegistry.h"

MC_BEGIN_NAMESPACE

class IMcConfigurableBeanFactory
        : public IMcBeanFactory
        , public IMcBeanDefinitionRegistry {
    
public:
    virtual ~IMcConfigurableBeanFactory() = default;
};

MC_END_NAMESPACE

MC_FORWARD_DECL_CLASS(IMcConfigurableBeanFactory)
