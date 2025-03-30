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
#include "McCrashHandler.h"

#include <QDateTime>
#include <QDebug>
#include <QRegularExpression>

#include "McGlobal.h"

#ifdef Q_OS_WIN
# include "google_breakpad/client/windows/handler/exception_handler.h"
#elif defined(Q_OS_LINUX)
# include "google_breakpad/client/linux/handler/exception_handler.h"
#endif

MC_DECL_PRIVATE_DATA2(McCrashHandler)
{
#ifdef Q_OS_WIN
    QScopedPointer<google_breakpad::ExceptionHandler> handler;
#endif

    bool isBackupEnable{false};
    QString backupDir;
    QString backupDirPattern;
    QString backupNamePattern;
};

#ifdef Q_OS_WIN
static bool CrashFilterCallback(void *context, EXCEPTION_POINTERS *exinfo, MDRawAssertionInfo *assertion)
{
    Q_UNUSED(context)
    Q_UNUSED(exinfo)
    Q_UNUSED(assertion)
    qCCritical(mcCore, "exception occurred, about to write to dump");
    return true;
}

static bool CrashMinidumpCallback(const wchar_t *dump_path, const wchar_t *minidump_id, void *context,
    EXCEPTION_POINTERS *exinfo, MDRawAssertionInfo *assertion, bool succeeded)
{
    Q_UNUSED(context)
    Q_UNUSED(exinfo)
    Q_UNUSED(assertion)
    qCCritical(mcCore, "dump write completed: '%s'. dump file path: '%ls/%ls.dmp'", succeeded ? "true" : "false",
        dump_path, minidump_id);

    do {
        if (!succeeded) {
            break;
        }

        auto data = static_cast<MC_PRIVATE_DATA_NAME(McCrashHandler) *>(context);
        if (!data->isBackupEnable) {
            break;
        }
        if (data->backupDir.isEmpty()) {
            break; //!< 容错
        }

        QDir srcDir(QString::fromWCharArray(dump_path));
        QString srcName = QString::fromWCharArray(minidump_id) + ".dmp";
        QString srcPath = srcDir.absoluteFilePath(srcName);

        QDir dstDir(data->backupDir);
        if (!dstDir.exists()) {
            dstDir.mkpath(data->backupDir);
        }
        QRegularExpression re(QStringLiteral(R"((.*)%\{time (.*?)\}(.*))"));
        if (!data->backupDirPattern.isEmpty()) {
            QString childDirName;
            auto match = re.match(data->backupDirPattern);
            if (match.hasMatch()) {
                auto list = match.capturedTexts();
                QDateTime dateTime = QDateTime::currentDateTime();
                childDirName = list.at(1) + dateTime.toString(list.at(2)) + list.at(3);
            } else {
                childDirName = data->backupDirPattern;
            }
            if (!dstDir.exists(childDirName)) {
                dstDir.mkdir(childDirName);
            }
            dstDir.cd(childDirName);
        }
        QString dstName;
        if (data->backupNamePattern.isEmpty()) {
            dstName = srcName;
        } else {
            auto match = re.match(data->backupNamePattern);
            if (match.hasMatch()) {
                auto list = match.capturedTexts();
                QDateTime dateTime = QDateTime::currentDateTime();
                dstName = list.at(1) + dateTime.toString(list.at(2)) + list.at(3);
            } else {
                dstName = data->backupNamePattern;
            }
        }

        QString dstPath = dstDir.absoluteFilePath(dstName);
        if (!QFile::rename(srcPath, dstPath)) {
            qCCritical(
                mcCore, "move dump failure. srcPath: '%s', dstPath: '%s'", qPrintable(srcPath), qPrintable(dstPath));
        }
    } while (false);

    return succeeded;
}
#endif

McCrashHandler::McCrashHandler() noexcept
{
    MC_NEW_PRIVATE_DATA(McCrashHandler);
}

McCrashHandler::McCrashHandler(const QString &dumpDir) noexcept
    : McCrashHandler()
{
    installHandler(dumpDir);
}

McCrashHandler::~McCrashHandler() {}

void McCrashHandler::installHandler(const QString &dumpDir) noexcept
{
    QDir dir(dumpDir);
    if (!dir.exists()) {
        dir.mkpath(dumpDir);
    }

    using namespace google_breakpad;
#ifdef Q_OS_WIN
    d->handler.reset(new ExceptionHandler(
        dumpDir.toStdWString(), CrashFilterCallback, CrashMinidumpCallback, d.get(), ExceptionHandler::HANDLER_ALL));
#elif defined(Q_OS_LINUX)
#endif
}

void McCrashHandler::enableBackup(
    const QString &backupDir, const QString &backupDirPattern, const QString &backupNamePattern) noexcept
{
    Q_ASSERT(!backupDir.isEmpty());

    d->isBackupEnable = true;
    d->backupDir = backupDir;
    d->backupDirPattern = backupDirPattern;
    d->backupNamePattern = backupNamePattern;
}
