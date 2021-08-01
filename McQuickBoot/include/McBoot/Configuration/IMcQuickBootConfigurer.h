#pragma once

#include "../McBootGlobal.h"
#include "../Utils/Response/McResponseHandlerFactory.h"

class IMcQuickBootConfigurer
{
public:
    virtual ~IMcQuickBootConfigurer() = default;

    virtual void addResponseHandler(McResponseHandlerRegistry &registry) noexcept = 0;
};

MC_DECL_METATYPE(IMcQuickBootConfigurer)
