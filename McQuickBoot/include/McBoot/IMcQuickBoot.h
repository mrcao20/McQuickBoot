#pragma once

#include "McBootGlobal.h"

MC_FORWARD_DECL_CLASS(IMcApplicationContext);

class IMcQuickBoot
{
public:
    virtual ~IMcQuickBoot() = default;
    
    virtual IMcApplicationContextPtr getApplicationContext() const noexcept = 0;
};

MC_DECL_POINTER(IMcQuickBoot);
