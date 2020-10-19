#pragma once

#include "../McGlobal.h"

MC_FORWARD_DECL_CLASS(IMcApplicationContext);

class IMcRelatableApplicationContext
{
public:
    virtual ~IMcRelatableApplicationContext() = default;
    
    virtual void addRelatedApplicationContext(IMcApplicationContextConstPtrRef appCtx) noexcept = 0;
    virtual void removeRelatedApplicationContext(IMcApplicationContextConstPtrRef appCtx) noexcept = 0;
    virtual QList<IMcApplicationContextPtr> getRelatedApplicationContexts() noexcept = 0;
};

MC_DECL_POINTER(IMcRelatableApplicationContext)
