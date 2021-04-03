#pragma once

#include "McFileDeviceAppender.h"

class MCLOGQT_EXPORT McConsoleAppender : public McFileDeviceAppender 
{
    Q_OBJECT
    MC_DECL_INIT(McConsoleAppender)
    MC_TYPELIST(McFileDeviceAppender)
public:
    Q_INVOKABLE McConsoleAppender();
    ~McConsoleAppender() override;

protected:
    void doFinished() noexcept override;
};

MC_DECL_METATYPE(McConsoleAppender)
