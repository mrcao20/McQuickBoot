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
#include "McLog/Appender/impl/McFileAppender.h"

#include <QCoreApplication>
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QDateTime>

MC_DECL_PRIVATE_DATA(McFileAppender)
QString dirPath;
QString fileNamePattern;
bool isAppend{true};
MC_PADDING_CLANG(7)
MC_DECL_PRIVATE_DATA_END

MC_INIT(McFileAppender)
MC_REGISTER_BEAN_FACTORY(McFileAppender)
MC_INIT_END

McFileAppender::McFileAppender() 
{
    MC_NEW_PRIVATE_DATA(McFileAppender);
}

McFileAppender::~McFileAppender() 
{
}

QString McFileAppender::dirPath() const noexcept 
{
    return d->dirPath;
}

void McFileAppender::setDirPath(const QString &val) noexcept 
{
    d->dirPath = Mc::toAbsolutePath(val);
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

void McFileAppender::doFinished() noexcept
{
    super::doFinished();

    checkExistsFile();
    tryNextFile();
}

QString McFileAppender::newFilePath() const noexcept 
{
    QRegularExpression re(R"((.*)%\{time (.*?)\}(.*))");
    auto match = re.match(d->fileNamePattern);
    
    QDir dir(d->dirPath);
    if(!match.hasMatch()) {
        return dir.absoluteFilePath(d->fileNamePattern);
    }
    
    auto list = match.capturedTexts();
    return dir.absoluteFilePath(list.at(1) 
                                + QDateTime::currentDateTime().toString(list.at(2)) 
                                + list.at(3));
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

    QRegularExpression re(R"((.*)%\{time (.*?)\}(.*))");
    auto match = re.match(d->fileNamePattern);

    QDir dir(d->dirPath);
    if (!dir.exists() && !dir.mkpath(d->dirPath)) {
        MC_PRINT_ERR("the dir path: %s is not exists. but cannot create!\n", qPrintable(d->dirPath));
        return false;
    }
    auto fileInfos = dir.entryInfoList(QDir::Files, QDir::Time); //!< 获取最新被修改的文件
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

    if (localFilePath.isEmpty() || localFilePath == oldFilePath) {
        localFilePath = newFilePath();
    }

    file->setFileName(localFilePath);
    QIODevice::OpenMode mode = QIODevice::WriteOnly | QIODevice::Text;
    if (d->isAppend)
        mode |= QIODevice::Append;
    if (!file->open(mode)) {
        MC_PRINT_ERR("error open file '%s' for write!!!\n", qPrintable(localFilePath));
        return false;
    }

    return true;
}
