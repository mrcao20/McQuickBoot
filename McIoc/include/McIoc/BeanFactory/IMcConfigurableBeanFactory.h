#pragma once

#include "IMcBeanFactory.h"
#include "IMcBeanDefinitionRegistry.h"

MC_FORWARD_DECL_CLASS(IMcPropertyConverter);

class IMcConfigurableBeanFactory : public IMcBeanFactory, public IMcBeanDefinitionRegistry
{
public:
    virtual ~IMcConfigurableBeanFactory() = default;

    virtual IMcPropertyConverterPtr getPropertyConverter() const noexcept = 0;
    virtual void setPropertyConverter(IMcPropertyConverterConstPtrRef converter) noexcept = 0;
};

MC_DECL_POINTER(IMcConfigurableBeanFactory)
