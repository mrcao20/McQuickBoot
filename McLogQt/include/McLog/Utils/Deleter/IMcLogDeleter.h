#pragma once

#include <McIoc/McGlobal.h>

class IMcLogDeleter
{
public:
    virtual ~IMcLogDeleter() = default;
};

MC_DECL_METATYPE(IMcLogDeleter)
