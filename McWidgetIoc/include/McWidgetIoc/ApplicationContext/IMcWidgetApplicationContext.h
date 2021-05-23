#pragma once

#include "../BeanFactory/IMcConfigurableWidgetBeanFactory.h"

class IMcWidgetApplicationContext : public IMcConfigurableWidgetBeanFactory
{
public:
    ~IMcWidgetApplicationContext() override = default;
};

MC_DECL_POINTER(IMcWidgetApplicationContext)
