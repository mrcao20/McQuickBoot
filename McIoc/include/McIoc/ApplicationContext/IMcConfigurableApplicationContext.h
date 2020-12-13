#pragma once

#include "IMcApplicationContext.h"
#include "IMcRelatableApplicationContext.h"

class IMcConfigurableApplicationContext
        : public IMcApplicationContext
        , public IMcRelatableApplicationContext
{
public:
    virtual ~IMcConfigurableApplicationContext() = default;
    
    virtual QVariant getBeanSelf(const QString &name, QThread *thread = nullptr) noexcept = 0;
    virtual bool containsBeanSelf(const QString &name) noexcept = 0;
    virtual bool isSingletonSelf(const QString &name) noexcept = 0;
    virtual void registerBeanDefinitionSelf(
            const QString &name,
            IMcBeanDefinitionConstPtrRef beanDefinition) noexcept = 0;
    virtual IMcBeanDefinitionPtr unregisterBeanDefinitionSelf(const QString &name) noexcept = 0;
    virtual QHash<QString, IMcBeanDefinitionPtr> getBeanDefinitionsSelf() noexcept = 0;
};

MC_DECL_POINTER(IMcConfigurableApplicationContext)
