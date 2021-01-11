#pragma once

#include "../IMcDestroyer.h"

class McNormalDestroyer : public IMcDestroyer
{
    MC_TYPELIST(IMcDestroyer);
public:
    ~McNormalDestroyer() override = default;
    
    void destroy() noexcept override
    { delete this; }
};

MC_DECL_METATYPE(McNormalDestroyer)
