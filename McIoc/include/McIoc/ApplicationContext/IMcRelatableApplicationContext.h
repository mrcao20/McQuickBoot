#pragma once

#include "../McGlobal.h"

MC_FORWARD_DECL_CLASS(IMcConfigurableBeanFactory);

class IMcRelatableApplicationContext
{
public:
    virtual ~IMcRelatableApplicationContext() = default;

    virtual void addRelatedBeanFactory(IMcConfigurableBeanFactoryConstPtrRef beanFac) noexcept = 0;
    virtual void removeRelatedBeanFactory(IMcConfigurableBeanFactoryConstPtrRef beanFac) noexcept = 0;
    virtual QList<IMcConfigurableBeanFactoryPtr> getRelatedBeanFactories() noexcept = 0;
};

MC_DECL_POINTER(IMcRelatableApplicationContext)
