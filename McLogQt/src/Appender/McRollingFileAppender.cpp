#include "McLog/Appender/impl/McRollingFileAppender.h"

#include <QCoreApplication>
#include <QDateTime>
#include <QDir>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QThread>

MC_DECL_PRIVATE_DATA(McRollingFileAppender)
QString backupDirPath;
QString backupDirPattern;
MC_DECL_PRIVATE_DATA_END

MC_INIT(McRollingFileAppender)
MC_INIT_END

McRollingFileAppender::McRollingFileAppender() 
{
    MC_NEW_PRIVATE_DATA(McRollingFileAppender);
}

McRollingFileAppender::~McRollingFileAppender() 
{
}

QString McRollingFileAppender::backupDirPath() const noexcept 
{
    return d->backupDirPath;
}

void McRollingFileAppender::setBackupDirPath(const QString &val) noexcept 
{
    d->backupDirPath = Mc::toAbsolutePath(val);
}

QString McRollingFileAppender::backupDirPattern() const noexcept 
{
    return d->backupDirPattern;
}

void McRollingFileAppender::setBackupDirPattern(const QString &val) noexcept 
{
    d->backupDirPattern = val;
}

void McRollingFileAppender::requestNextFile() noexcept
{
    QMetaObject::invokeMethod(this, MC_STRINGIFY(nextFile), Qt::QueuedConnection);
}

void McRollingFileAppender::forceRequestNextFile() noexcept
{
    if (thread() == QThread::currentThread()) {
        nextFile();
    } else {
        QMetaObject::invokeMethod(this, MC_STRINGIFY(nextFile), Qt::BlockingQueuedConnection);
    }
}

void McRollingFileAppender::doFinished() noexcept
{
    McFileAppender::doFinished();

    if(backupDirPath().isEmpty())
        setBackupDirPath("./backup/");
    if(backupDirPattern().isEmpty())
        setBackupDirPattern("%{time yyyy-MM-dd}");
}

void McRollingFileAppender::tryNextFile() noexcept 
{
    if(!isNewNextFile()) {
        return;
    }

    nextFile();
}

void McRollingFileAppender::nextFile() noexcept
{
    if (device().isNull()) {
        return;
    }

    auto file = device().staticCast<QFile>();

    auto oldFilePath = file->fileName();
    auto newFilePath = this->newFilePath();
    if (newFilePath == oldFilePath) {
        return;
    }
    auto mode = file->openMode();

    file->close();

    auto backupPath = newBackupPath(oldFilePath);
    QDir dir(backupPath);
    if (!dir.exists() && !dir.mkpath(backupPath)) {
        MC_PRINT_ERR("mk the backup path: %s failure.\n", qPrintable(backupPath));
        return;
    }
    QFile::rename(oldFilePath, dir.absoluteFilePath(QFileInfo(oldFilePath).fileName()));

    file->setFileName(newFilePath);
    if (!file->open(mode)) {
        MC_PRINT_ERR("error open file '%s' for write!!!\n", qPrintable(newFilePath));
        return;
    }
}

QString McRollingFileAppender::newBackupPath(const QString &oldFilePath) const noexcept 
{
    QRegularExpression re(R"((.*)%\{time (.*?)\}(.*))");
    auto match = re.match(d->backupDirPattern);
    
    QDir dir(d->backupDirPath);
    if(!match.hasMatch()) {
        return dir.absoluteFilePath(d->backupDirPattern);
    }
    
    auto list = match.capturedTexts();
    QFileInfo fileInfo(oldFilePath);
#if QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
    auto dateTime = fileInfo.birthTime();
    if(!dateTime.isValid()) {
        dateTime = fileInfo.metadataChangeTime();
    }
#else
    auto dateTime = fileInfo.created();
#endif
    if(!dateTime.isValid()) {
        qCritical("failed get birth time of the file: %s\n", qPrintable(oldFilePath));
    }
    return dir.absoluteFilePath(list.at(1) 
                                + dateTime.toString(list.at(2)) 
                                + list.at(3));
}
