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
#include "McLogPackager.h"

#include <QDateTime>
#include <QDir>
#include <QRegularExpression>
#include <QTimer>

#include <McCore/Zip/McCompressor.h>

MC_AUTO_INIT(McLogPackager)
MC_INIT_END

MC_DECL_PRIVATE_DATA(McLogPackager)
QList<QString> scanPaths;
int maxDepth{1};
QString age{"5D"};
int ageNumeric{5};
QList<QString> filters;
QString dstPath;
QString fileNamePattern;
MC_DECL_PRIVATE_DATA_END

McLogPackager::McLogPackager() noexcept
{
    MC_NEW_PRIVATE_DATA(McLogPackager);
}

McLogPackager::~McLogPackager() {}

void McLogPackager::execute() noexcept
{
    QMap<QString, QStringList> filePaths;
    for (auto &scanPath : qAsConst(d->scanPaths)) {
        auto path = Mc::toAbsolutePath(scanPath);
        auto files = checkFiles(0, path);
        if (files.isEmpty()) {
            continue;
        }
        filePaths[path].append(files);
    }
    if (filePaths.isEmpty()) {
        return;
    }
    auto tarPath = newFilePath();
    if (!McCompressor::compressFiles(tarPath, filePaths)) {
        return;
    }
    for (auto &files : qAsConst(filePaths)) {
        for (auto &file : files) {
            qCInfo(mcLog()) << "The file has packed. deleted:" << file;
            QFile::remove(file);
        }
    }
}

void McLogPackager::buildFinished() noexcept
{
    if (!d->age.endsWith("D", Qt::CaseInsensitive)) {
        MC_PRINT_ERR("age must endsWith 'D' for McLogDeleter property\n");
        return;
    }
    auto tmp = d->age;
    tmp.remove(tmp.length() - 1, 1);
    bool isOk = false;
    auto num = tmp.toInt(&isOk);
    if (isOk) {
        d->ageNumeric = num;
    }
    d->dstPath = Mc::toAbsolutePath(d->dstPath);
}

QStringList McLogPackager::checkFiles(int depth, const QString &path) noexcept
{
    auto curDepth = depth + 1;
    if (curDepth > d->maxDepth) {
        return QStringList();
    }
    QStringList filePaths;
    QDir dir(path);
    auto fileInfos = dir.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
    for (auto &fileInfo : qAsConst(fileInfos)) {
        auto absPath = fileInfo.absoluteFilePath();
        if (fileInfo.isDir()) {
            filePaths.append(checkFiles(curDepth, absPath));
            continue;
        }
        if (!fileNameCheck(fileInfo.fileName())) {
            continue;
        }
        auto fileDate = fileInfo.lastModified().date();
        auto curDate = QDate::currentDate();
        auto day = fileDate.daysTo(curDate);
        if (day > d->ageNumeric) {
            filePaths.append(absPath);
        }
    }
    return filePaths;
}

bool McLogPackager::fileNameCheck(const QString &fileName) noexcept
{
    if (d->filters.isEmpty()) {
        return true;
    }
    for (auto &filter : qAsConst(d->filters)) {
        QRegularExpression reg(filter);
        auto match = reg.match(fileName);
        if (!match.capturedTexts().isEmpty()) {
            return true;
        }
    }
    return false;
}

QString McLogPackager::newFilePath() const noexcept
{
    QRegularExpression re(R"((.*)%\{time (.*?)\}(.*))");
    auto match = re.match(d->fileNamePattern);

    QDir dir(d->dstPath);
    if (!match.hasMatch()) {
        return dir.absoluteFilePath(d->fileNamePattern);
    }

    auto list = match.capturedTexts();
    return dir.absoluteFilePath(list.at(1) + QDateTime::currentDateTime().toString(list.at(2)) + list.at(3));
}

#include "moc_McLogPackager.cpp"
