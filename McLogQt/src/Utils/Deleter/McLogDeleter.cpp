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
#include "McLog/Utils/Deleter/McLogDeleter.h"

#include <QRegularExpression>
#include <QDateTime>
#include <QTimer>
#include <QDir>

#include "McLog/McLogGlobal.h"

MC_STATIC()
MC_REGISTER_BEAN_FACTORY(McLogDeleter)
MC_STATIC_END

MC_DECL_PRIVATE_DATA(McLogDeleter)
QList<QString> basePaths;
int maxDepth{1};
QString age{"14D"};
int ageNumeric{14};
QList<QString> filters;
MC_DECL_PRIVATE_DATA_END

McLogDeleter::McLogDeleter(QObject *parent) noexcept
    : QObject(parent)
{
    MC_NEW_PRIVATE_DATA(McLogDeleter);
}

McLogDeleter::~McLogDeleter()
{
}

void McLogDeleter::finished() noexcept
{
    if(!d->age.endsWith("D", Qt::CaseInsensitive)) {
        MC_PRINT_ERR("age must endsWith 'D' for McLogDeleter property\n");
        return;
    }
    auto tmp = d->age;
    tmp.remove(tmp.length() - 1, 1);
    bool isOk = false;
    auto num = tmp.toInt(&isOk);
    if(isOk) {
        d->ageNumeric = num;
    }
}

void McLogDeleter::execute() noexcept
{
    for (auto &basePath : qAsConst(d->basePaths)) {
        auto path = Mc::toAbsolutePath(basePath);
        checkFiles(0, path);
    }
}

void McLogDeleter::checkFiles(int depth, const QString &path) noexcept
{
    auto curDepth = depth + 1;
    if(curDepth > d->maxDepth) {
        return;
    }
    QDir dir(path);
    auto fileInfos = dir.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
    for(auto &fileInfo : qAsConst(fileInfos)) {
        auto absPath = fileInfo.absoluteFilePath();
        if(fileInfo.isDir()) {
            checkFiles(curDepth, absPath);
            continue;
        }
        if(!fileNameCheck(fileInfo.fileName())) {
            continue;
        }
        auto fileDate = fileInfo.lastModified().date();
        auto curDate = QDate::currentDate();
        auto day = fileDate.daysTo(curDate);
        if(day > d->ageNumeric) {
            qInfo() << "The file has expired. deleted:" << absPath;
            QFile::remove(absPath);
        }
    }
    fileInfos = dir.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
    if(fileInfos.isEmpty()) {
        dir.rmpath(path);
    }
}

bool McLogDeleter::fileNameCheck(const QString &fileName) noexcept
{
    if (d->filters.isEmpty()) {
        return true;
    }
    for(auto &filter : qAsConst(d->filters)) {
        QRegularExpression reg(filter);
        auto match = reg.match(fileName);
        if(!match.capturedTexts().isEmpty()) {
            return true;
        }
    }
    return false;
}

#include "moc_McLogDeleter.cpp"
