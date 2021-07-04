#include "McLog/Appender/impl/McDailyRollingFileAppender.h"

MC_INIT(McDailyRollingFileAppender)
MC_REGISTER_BEAN_FACTORY(McDailyRollingFileAppender)
MC_INIT_END

McDailyRollingFileAppender::McDailyRollingFileAppender() 
{
}

void McDailyRollingFileAppender::doFinished() noexcept
{
    McByDayRollingFileAppender::doFinished();

    setDay(1);
}
