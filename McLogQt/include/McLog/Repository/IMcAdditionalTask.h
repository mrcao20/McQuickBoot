#pragma once

#include <McIoc/McGlobal.h>

class IMcAdditionalTask
{
public:
    virtual ~IMcAdditionalTask() = default;

    virtual void execute() noexcept = 0;
};

MC_DECL_METATYPE(IMcAdditionalTask)
