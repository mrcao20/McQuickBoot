#pragma once

#include "../IMcCallback.h"

#include "../../../McBootMacroGlobal.h"

class MCQUICKBOOT_EXPORT McAbstractAsyncCallback : public IMcCallback
{
    MC_TYPELIST(IMcCallback)
public:
    void destroy() noexcept override;
};

MC_DECL_METATYPE(McAbstractAsyncCallback)
