#pragma once

#include "../BeanFactory/IMcConfigurableBeanFactory.h"
#include "IMcRefreshableApplicationContext.h"

class IMcApplicationContext
        : public IMcConfigurableBeanFactory
        , public IMcRefreshableApplicationContext 
{
public:
    ~IMcApplicationContext() override = default;
};

MC_DECL_POINTER(IMcApplicationContext)
