#include "McLog/Appender/impl/McBySizeDayRollingFileAppender.h"

#include <QFile>

#include "McLog/Utils/McFileUtils.h"

MC_INIT(McBySizeDayRollingFileAppender)
MC_REGISTER_BEAN_FACTORY(McBySizeDayRollingFileAppender)
MC_INIT_END

MC_DECL_PRIVATE_DATA(McBySizeDayRollingFileAppender)
int day{1};
MC_DECL_PRIVATE_DATA_END

McBySizeDayRollingFileAppender::McBySizeDayRollingFileAppender()
{
    MC_NEW_PRIVATE_DATA(McBySizeDayRollingFileAppender);
}

McBySizeDayRollingFileAppender::~McBySizeDayRollingFileAppender() {}

int McBySizeDayRollingFileAppender::day() const noexcept
{
    return d->day;
}

void McBySizeDayRollingFileAppender::setDay(int val) noexcept
{
    d->day = val;
}

bool McBySizeDayRollingFileAppender::isNewNextFile() noexcept
{
    if (McSizeRollingFileAppender::isNewNextFile()) {
        return true;
    }
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
