#pragma once

#include "../IMcConfigurationParser.h"

class McQuickBootConfigurerParser : public IMcConfigurationParser
{
    MC_DECL_INIT(McQuickBootConfigurerParser)
public:
    void parse(const QVariant &var) noexcept;
};
