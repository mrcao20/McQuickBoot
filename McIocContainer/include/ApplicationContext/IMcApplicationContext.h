#pragma once

#include "../BeanFactory/IMcConfigurableBeanFactory.h"
#include "../ApplicationContext/IMcRefreshableApplicationContext.h"

MC_BEGIN_NAMESPACE

class IMcApplicationContext
        : public IMcConfigurableBeanFactory
        , public IMcRefreshableApplicationContext {
    
public:
    ~IMcApplicationContext() override = default;
};

MC_END_NAMESPACE

MC_FORWARD_DECL_CLASS(IMcApplicationContext)
