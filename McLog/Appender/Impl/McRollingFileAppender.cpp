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
#include "McRollingFileAppender.h"

#include <QCoreApplication>
#include <QDateTime>
#include <QDir>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QScopeGuard>
#include <QThread>

MC_DECL_PRIVATE_DATA(McRollingFileAppender)
QString backupDirPath;
QString backupDirPattern;
MC_DECL_PRIVATE_DATA_END

McRollingFileAppender::McRollingFileAppender() noexcept
{
    MC_NEW_PRIVATE_DATA(McRollingFileAppender);
}

McRollingFileAppender::~McRollingFileAppender() {}

QString McRollingFileAppender::backupDirPath() const noexcept
{
    return d->backupDirPath;
}

void McRollingFileAppender::setBackupDirPath(const QString &val) noexcept
{
    d->backupDirPath = val;
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
    QMetaObject::invokeMethod(this, &McRollingFileAppender::nextFile, Qt::QueuedConnection);
}

bool McRollingFileAppender::forceRequestNextFile() noexcept
{
    bool ret = false;
    if (thread() == QThread::currentThread()) {
        ret = nextFile();
    } else {
        QMetaObject::invokeMethod(this, &McRollingFileAppender::nextFile, Qt::BlockingQueuedConnection, &ret);
    }
    return ret;
}

void McRollingFileAppender::buildFinished() noexcept
{
    super::buildFinished();

    if (backupDirPath().isEmpty())
        setBackupDirPath("./backup/");
    if (backupDirPattern().isEmpty())
        setBackupDirPattern("%{time yyyy-MM-dd}");

    tryNextFile();
}

void McRollingFileAppender::writeBefore() noexcept
{
    auto cleanup = qScopeGuard([this]() { tryNextFile(); });
    if (!useLockFile()) {
        return;
    }
    auto file = device().staticCast<QFile>();
    if (file.isNull()) {
        return;
    }
    auto lastFilePath = getLastFilePath();
    if (lastFilePath == file->fileName()) {
        return;
    }
    nextFile();
}

void McRollingFileAppender::writeAfter() noexcept
{
    tryNextFile();
}

void McRollingFileAppender::tryNextFile() noexcept
{
    if (!isRolling()) {
        return;
    }

    nextFile();
}

bool McRollingFileAppender::nextFile() noexcept
{
    if (device().isNull()) {
        return false;
    }

    auto file = device().staticCast<QFile>();

    auto oldFilePath = file->fileName();
    file->close();

    auto backupPath = newBackupPath(oldFilePath);
    QDir dir(backupPath);
    if (!dir.exists() && !dir.mkpath(backupPath)) {
        MC_PRINT_ERR("mk the backup path: %s failure.\n", qUtf8Printable(backupPath));
        return false;
    }
    auto ret = QFile::rename(oldFilePath, dir.absoluteFilePath(QFileInfo(oldFilePath).fileName()));

    if (!checkExistsFile()) {
        ret = false;
    }

    return ret;
}

QString McRollingFileAppender::newBackupPath(const QString &oldFilePath) const noexcept
{
    QRegularExpression re(R"((.*)%\{time (.*?)\}(.*))");
    auto match = re.match(d->backupDirPattern);

    QDir dir(Mc::toAbsolutePath(d->backupDirPath));
    if (!match.hasMatch()) {
        return dir.absoluteFilePath(d->backupDirPattern);
    }

    auto list = match.capturedTexts();
    QFileInfo fileInfo(oldFilePath);
    auto dateTime = fileInfo.birthTime();
    if (!dateTime.isValid()) {
        dateTime = fileInfo.metadataChangeTime();
    }
    if (!dateTime.isValid()) {
        qCCritical(mcLog(), "failed get birth time of the file: %s\n", qUtf8Printable(oldFilePath));
    }
    return dir.absoluteFilePath(list.at(1) + dateTime.toString(list.at(2)) + list.at(3));
}
