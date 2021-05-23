#pragma once

#include <McIoc/BeanFactory/IMcBeanDefinitionRegistry.h>

#include "IMcWidgetBeanFactory.h"

class IMcConfigurableWidgetBeanFactory : public IMcWidgetBeanFactory,
                                         public IMcBeanDefinitionRegistry
{
public:
    virtual ~IMcConfigurableWidgetBeanFactory() = default;
};

MC_DECL_POINTER(IMcConfigurableWidgetBeanFactory)
