#pragma once

#include "McAbstractAppender.h"

class MCLOGQT_EXPORT McFileDeviceAppender : public McAbstractAppender 
{
    Q_OBJECT
    MC_DECL_INIT(McFileDeviceAppender)
    MC_DEFINE_TYPELIST(MC_DECL_TYPELIST(McAbstractAppender))
public:
    Q_INVOKABLE McFileDeviceAppender();
    ~McFileDeviceAppender() override;
    
protected:
    void flush() noexcept override;
    
    virtual void tryNextFile() noexcept;
};

MC_DECL_METATYPE(McFileDeviceAppender)
