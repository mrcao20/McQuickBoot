#include "McLog/Appender/impl/McDailyRollingFileAppender.h"

#include <QFile>
#include <QFileInfo>
#include <QDateTime>

MC_INIT(McDailyRollingFileAppender)
MC_REGISTER_BEAN_FACTORY(McDailyRollingFileAppender)
MC_INIT_END

McDailyRollingFileAppender::McDailyRollingFileAppender() 
{
}

bool McDailyRollingFileAppender::isNewNextFile() noexcept 
{
    if(device().isNull()) {
        return false;
    }
    auto file = device().staticCast<QFile>();
    if(!file->isOpen()) {
        return false;
    }
    auto filePath = file->fileName();
    QFileInfo fileInfo(filePath);
#if QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
    auto dateTime = fileInfo.birthTime();
    if(!dateTime.isValid()) {
        dateTime = fileInfo.metadataChangeTime();
    }
#else
    auto dateTime = fileInfo.created();
#endif
    if(!dateTime.isValid()) {
        qCritical("failed get birth time of the file: %s\n", qPrintable(filePath));
        return true;
    }
    auto curDateTime = QDateTime::currentDateTime();
    return qAbs(curDateTime.daysTo(dateTime)) > 0;
}
