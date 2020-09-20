#pragma once

#include "McAbstractFormatAppender.h"

class MCLOGQT_EXPORT McFileDeviceAppender : public McAbstractFormatAppender 
{
    Q_OBJECT
    MC_DECL_INIT(McFileDeviceAppender)
    MC_DEFINE_TYPELIST(MC_DECL_TYPELIST(McAbstractFormatAppender))
public:
    Q_INVOKABLE McFileDeviceAppender();
    ~McFileDeviceAppender() override;
    
protected:
    void writeBefore() noexcept override;
    void writeAfter() noexcept override;
    
    virtual void tryNextFile() noexcept;
};

MC_DECL_METATYPE(McFileDeviceAppender)
