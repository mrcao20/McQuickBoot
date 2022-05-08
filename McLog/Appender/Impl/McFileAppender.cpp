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
#include "McFileAppender.h"

#include <QCoreApplication>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QRegularExpression>
#include <QRegularExpressionMatch>

MC_DECL_PRIVATE_DATA(McFileAppender)
QString dirPath;
QString fileNamePattern;
bool isAppend{true};
MC_DECL_PRIVATE_DATA_END

MC_AUTO_INIT(McFileAppender)
MC_INIT_END

McFileAppender::McFileAppender() noexcept
{
    MC_NEW_PRIVATE_DATA(McFileAppender);
}

McFileAppender::~McFileAppender() {}

QString McFileAppender::dirPath() const noexcept
{
    return d->dirPath;
}

void McFileAppender::setDirPath(const QString &val) noexcept
{
    d->dirPath = val;
}

QString McFileAppender::fileNamePattern() const noexcept
{
    return d->fileNamePattern;
}

void McFileAppender::setFileNamePattern(const QString &val) noexcept
{
    d->fileNamePattern = val;
}

bool McFileAppender::isAppend() const noexcept
{
    return d->isAppend;
}

void McFileAppender::setAppend(bool val) noexcept
{
    d->isAppend = val;
}

void McFileAppender::buildFinished() noexcept
{
    super::buildFinished();

    checkExistsFile();
}

void McFileAppender::writeBefore() noexcept {}

void McFileAppender::writeAfter() noexcept {}

QString McFileAppender::newFilePath() const noexcept
{
    QRegularExpression re(R"((.*)%\{time (.*?)\}(.*))");
    auto match = re.match(d->fileNamePattern);

    QDir dir(Mc::toAbsolutePath(d->dirPath));
    if (!match.hasMatch()) {
        return dir.absoluteFilePath(d->fileNamePattern);
    }

    auto list = match.capturedTexts();
    return dir.absoluteFilePath(list.at(1) + QDateTime::currentDateTime().toString(list.at(2)) + list.at(3));
}

bool McFileAppender::checkExistsFile() noexcept
{
    QString oldFilePath;
    auto file = device().staticCast<QFile>();
    if (file.isNull()) {
        file = QSharedPointer<QFile>::create();
        setDevice(file);
    } else {
        oldFilePath = file->fileName();
    }

    auto localFilePath = getLastFilePath();

    if (localFilePath.isEmpty() || localFilePath == oldFilePath) {
        localFilePath = newFilePath();
    }

    file->setFileName(localFilePath);
    QIODevice::OpenMode mode = QIODevice::WriteOnly | QIODevice::Text;
    if (d->isAppend)
        mode |= QIODevice::Append;
    if (!file->open(mode)) {
        MC_PRINT_ERR("error open file '%s' for write!!!\n", qUtf8Printable(localFilePath));
        return false;
    }

    return true;
}

QString McFileAppender::getLastFilePath() const noexcept
{
    QRegularExpression re(R"((.*)%\{time (.*?)\}(.*))");
    auto match = re.match(d->fileNamePattern);

    auto dirPath = Mc::toAbsolutePath(d->dirPath);
    QDir dir(dirPath);
    if (!dir.exists() && !dir.mkpath(dirPath)) {
        MC_PRINT_ERR("the dir path: %s is not exists. but cannot create!\n", qUtf8Printable(dirPath));
        return QString();
    }
    auto fileInfos = dir.entryInfoList(QDir::Files);
    std::sort(fileInfos.begin(), fileInfos.end(), [](const QFileInfo &f1, const QFileInfo &f2) {
        auto d1 = f1.birthTime();
        if (!d1.isValid()) {
            d1 = f1.metadataChangeTime();
        }
        auto d2 = f2.birthTime();
        if (!d2.isValid()) {
            d2 = f2.metadataChangeTime();
        }
        return d1 > d2;
    }); //!< 按照文件创建时间降序排序
    QString localFilePath;
    for (auto &fileInfo : qAsConst(fileInfos)) {
        auto fileName = fileInfo.fileName();
        if (match.hasMatch()) { //!< 如果匹配成功，则list的长度一定为4
            auto list = match.capturedTexts();
            if (!fileName.startsWith(list.at(1)) || !fileName.endsWith(list.at(3))) {
                continue;
            }
            fileName.remove(fileName.size() - list.at(3).size(), list.at(3).size()); //!< 移除末尾
            fileName.remove(0, list.at(1).size()); //!< 移除开头
            QDateTime dateTime = QDateTime::fromString(fileName, list.at(2));
            if (!dateTime.isValid()) {
                continue;
            }
            localFilePath = fileInfo.filePath();
            break;
        } else {
            if (d->fileNamePattern != fileName) {
                continue;
            }
            localFilePath = fileInfo.filePath();
            break;
        }
    }
    return localFilePath;
}
