#pragma once

#include "McBySizeDayRollingFileAppender.h"

class MCLOGQT_EXPORT McBySizeDailyRollingFileAppender : public McBySizeDayRollingFileAppender
{
    Q_OBJECT
    MC_DECL_INIT(McBySizeDailyRollingFileAppender)
    MC_TYPELIST(McBySizeDayRollingFileAppender)
public:
    McBySizeDailyRollingFileAppender();

protected:
    void doFinished() noexcept override;
};

MC_DECL_METATYPE(McBySizeDailyRollingFileAppender)
