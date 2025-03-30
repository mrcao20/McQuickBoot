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
#include "McLogDeleter.h"

#include <QDateTime>
#include <QDir>
#include <QRegularExpression>
#include <QTimer>

MC_AUTO_INIT(McLogDeleter)
#ifdef MC_USE_QT5
mcRegisterContainer<QList<QString>>();
#endif
MC_INIT_END

MC_DECL_PRIVATE_DATA(McLogDeleter)
QList<QString> basePaths;
int maxDepth{1};
QString age{QStringLiteral("14D")};
int ageNumeric{14};
QList<QString> filters;
MC_DECL_PRIVATE_DATA_END

McLogDeleter::McLogDeleter() noexcept
{
    MC_NEW_PRIVATE_DATA(McLogDeleter);
}

McLogDeleter::~McLogDeleter() {}

void McLogDeleter::execute() noexcept
{
    for (auto &basePath : qAsConst(d->basePaths)) {
        auto path = Mc::toAbsolutePath(basePath);
        checkFiles(0, path);
    }
}

void McLogDeleter::buildFinished() noexcept
{
    if (!d->age.endsWith('D', Qt::CaseInsensitive)) {
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
}

void McLogDeleter::checkFiles(int depth, const QString &path) noexcept
{
    auto curDepth = depth + 1;
    if (curDepth > d->maxDepth) {
        return;
    }
    QDir dir(path);
    auto fileInfos = dir.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
    for (auto &fileInfo : qAsConst(fileInfos)) {
        auto absPath = fileInfo.absoluteFilePath();
        if (fileInfo.isDir()) {
            checkFiles(curDepth, absPath);
            continue;
        }
        if (!fileNameCheck(fileInfo.fileName())) {
            continue;
        }
        auto fileDate = fileInfo.lastModified().date();
        auto curDate = QDate::currentDate();
        auto day = fileDate.daysTo(curDate);
        if (day > d->ageNumeric) {
            qCInfo(mcLog()) << "The file has expired. deleted:" << absPath;
            QFile::remove(absPath);
        }
    }
    fileInfos = dir.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
    if (fileInfos.isEmpty()) {
        dir.rmpath(path);
    }
}

bool McLogDeleter::fileNameCheck(const QString &fileName) noexcept
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

#include "moc_McLogDeleter.cpp"
