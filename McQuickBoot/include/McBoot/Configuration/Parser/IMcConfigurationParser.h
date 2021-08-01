#pragma once

#include "../../McBootGlobal.h"

class IMcConfigurationParser
{
public:
    virtual ~IMcConfigurationParser() = default;

    virtual void parse(const QVariant &var) noexcept = 0;
};

MC_DECL_METATYPE(IMcConfigurationParser)
