/*
 * MIT License
 *
 * Copyright (c) 2021 mrcao20
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
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
