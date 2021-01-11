#pragma once

#include "McIoc/McGlobal.h"

class Interface
{
    MC_TYPELIST()
public:
    virtual ~Interface() = default;
    
    virtual void say() noexcept = 0;
};

MC_DECL_METATYPE(Interface)
