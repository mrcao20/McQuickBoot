#pragma once

#include "McByDayRollingFileAppender.h"

class MCLOGQT_EXPORT McDailyRollingFileAppender : public McByDayRollingFileAppender
{
    Q_OBJECT
    MC_DECL_INIT(McDailyRollingFileAppender)
    MC_TYPELIST(McByDayRollingFileAppender)
public:
    McDailyRollingFileAppender();

protected:
    void doFinished() noexcept override;
};

MC_DECL_METATYPE(McDailyRollingFileAppender)
