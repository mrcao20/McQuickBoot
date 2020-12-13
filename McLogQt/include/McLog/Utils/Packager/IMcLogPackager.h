#pragma once

#include "McIoc/McGlobal.h"

class IMcLogPackager
{
public:
    virtual ~IMcLogPackager() = default;
};

MC_DECL_METATYPE(IMcLogPackager)
