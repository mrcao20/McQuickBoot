#include "McLog/Appender/impl/McByDayRollingFileAppender.h"

#include <QFile>

#include "McLog/Utils/McFileUtils.h"

MC_INIT(McByDayRollingFileAppender)
MC_REGISTER_BEAN_FACTORY(McByDayRollingFileAppender)
MC_INIT_END

MC_DECL_PRIVATE_DATA(McByDayRollingFileAppender)
int day{1};
MC_DECL_PRIVATE_DATA_END

McByDayRollingFileAppender::McByDayRollingFileAppender()
{
    MC_NEW_PRIVATE_DATA(McByDayRollingFileAppender);
}

McByDayRollingFileAppender::~McByDayRollingFileAppender() {}

int McByDayRollingFileAppender::day() const noexcept
{
    return d->day;
}

void McByDayRollingFileAppender::setDay(int val) noexcept
{
    d->day = val;
}

bool McByDayRollingFileAppender::isNewNextFile() noexcept
{
    if (device().isNull()) {
        return false;
    }
    auto file = device().staticCast<QFile>();
    if (!file->isOpen()) {
        return false;
    }
    auto filePath = file->fileName();
    return McFileUtils::judgeDate(filePath, d->day);
}
