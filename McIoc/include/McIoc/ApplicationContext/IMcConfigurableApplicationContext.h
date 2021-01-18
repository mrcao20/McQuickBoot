#pragma once

#include "IMcApplicationContext.h"
#include "IMcRelatableApplicationContext.h"

class IMcConfigurableApplicationContext : public IMcApplicationContext,
                                          public IMcRelatableApplicationContext
{
public:
    virtual ~IMcConfigurableApplicationContext() = default;
};

MC_DECL_POINTER(IMcConfigurableApplicationContext)
