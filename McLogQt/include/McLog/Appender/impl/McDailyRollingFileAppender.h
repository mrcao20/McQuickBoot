#pragma once

#include "McRollingFileAppender.h"

class MCLOGQT_EXPORT McDailyRollingFileAppender : public McRollingFileAppender 
{
    Q_OBJECT
    MC_DECL_INIT(McDailyRollingFileAppender)
    MC_TYPELIST(McRollingFileAppender)
public:
    Q_INVOKABLE McDailyRollingFileAppender();
    
protected:
    bool isNewNextFile() noexcept override;
};

MC_DECL_METATYPE(McDailyRollingFileAppender)
