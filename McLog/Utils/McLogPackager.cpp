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
