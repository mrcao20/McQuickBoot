#pragma once

#include "../McGlobal.h"

MC_FORWARD_DECL_CLASS(IMcBeanDefinitionReader);

class IMcReadableApplicationContext {
public:
    virtual ~IMcReadableApplicationContext() = default;
    
    virtual void readBeans() noexcept = 0;
    
    virtual void setReader(IMcBeanDefinitionReaderConstPtrRef reader) noexcept = 0;
};

MC_DECL_POINTER(IMcReadableApplicationContext)
