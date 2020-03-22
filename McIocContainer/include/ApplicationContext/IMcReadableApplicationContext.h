#pragma once

#include "../McGlobal.h"

MC_BEGIN_NAMESPACE

MC_DECL_POINTER(IMcBeanDefinitionReader);

class IMcReadableApplicationContext {
public:
    virtual ~IMcReadableApplicationContext() = default;
    
    virtual void readBeans() noexcept = 0;
    
    virtual void setReader(IMcBeanDefinitionReaderConstPtrRef reader) noexcept = 0;
};

MC_END_NAMESPACE

MC_FORWARD_DECL_CLASS(IMcReadableApplicationContext)
