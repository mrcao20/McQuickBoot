#include "McLog/Utils/McFileUtils.h"

#include <QDateTime>
#include <QFileInfo>

bool McFileUtils::judgeDate(const QString &filePath, int day) noexcept
{
    QFileInfo fileInfo(filePath);
    if (!fileInfo.exists()) {
        return true;
    }
#if QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
    auto dateTime = fileInfo.birthTime();
    if (!dateTime.isValid()) {
        dateTime = fileInfo.metadataChangeTime();
    }
#else
    auto dateTime = fileInfo.created();
#endif
    if (!dateTime.isValid()) {
        qCritical("failed get birth time of the file: %s\n", qPrintable(filePath));
        return true;
    }
    auto curDateTime = QDateTime::currentDateTime();
    return qAbs(curDateTime.daysTo(dateTime)) >= day;
}
