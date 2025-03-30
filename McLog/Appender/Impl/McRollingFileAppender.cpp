/*
 * Copyright (c) 2021 mrcao20/mrcao20@163.com
 * McQuickBoot is licensed under Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at:
 *          http://license.coscl.org.cn/MulanPSL2
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
 * EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
 * MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
 * See the Mulan PSL v2 for more details.
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
    QRegularExpression re(QStringLiteral(R"((.*)%\{time (.*?)\}(.*))"));
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
