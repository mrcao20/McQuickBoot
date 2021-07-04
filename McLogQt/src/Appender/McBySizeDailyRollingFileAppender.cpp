#include "McLog/Appender/impl/McBySizeDailyRollingFileAppender.h"

MC_INIT(McBySizeDailyRollingFileAppender)
MC_REGISTER_BEAN_FACTORY(McBySizeDailyRollingFileAppender)
MC_INIT_END

McBySizeDailyRollingFileAppender::McBySizeDailyRollingFileAppender() {}

void McBySizeDailyRollingFileAppender::doFinished() noexcept
{
    McBySizeDayRollingFileAppender::doFinished();

    setDay(1);
}
