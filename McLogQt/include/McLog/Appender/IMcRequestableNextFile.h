#pragma once

#include <McIoc/McGlobal.h>

class IMcRequestableNextFile
{
public:
    virtual ~IMcRequestableNextFile() = default;

    virtual void requestNextFile() noexcept = 0;
    virtual void forceRequestNextFile() noexcept = 0;
};

MC_DECL_METATYPE(IMcRequestableNextFile)
