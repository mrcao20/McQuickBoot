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
#include "McCompressor.h"

#include <QDir>
#include <QFile>
#include <QMap>
#include <QScopeGuard>
#include <QtGlobal>

#include "McGlobal.h"
#include "zip.h"

namespace {
bool compressFile(zipFile zFile, const QString &filePath, const QString &destPath) noexcept
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qCCritical(mcCore(), "cannot open file when create zip: %s. error str: %s", qUtf8Printable(filePath),
            qUtf8Printable(file.errorString()));
        return false;
    }
    zip_fileinfo zFileInfo = {0};
    if (zipOpenNewFileInZip(
            zFile, destPath.toUtf8().constData(), &zFileInfo, NULL, 0, NULL, 0, NULL, 0, Z_DEFAULT_COMPRESSION)
        != ZIP_OK) {
        return false;
    }
    if (zipWriteInFileInZip(zFile, file.readAll().constData(), static_cast<unsigned int>(file.size())) != ZIP_OK) {
        zipCloseFileInZip(zFile);
        return false;
    }
    return zipCloseFileInZip(zFile) == ZIP_OK;
}
} // namespace

bool McCompressor::compressFiles(
    const QString &fileCompressed, const QStringList &files, const QString &rootPath) noexcept
{
    QMap<QString, QStringList> filePaths;
    filePaths.insert(rootPath, files);
    return compressFiles(fileCompressed, filePaths);
}

bool McCompressor::compressFiles(const QString &fileCompressed, const QMap<QString, QStringList> &filePaths) noexcept
{
    QDir().mkpath(QFileInfo(fileCompressed).absolutePath());
    zipFile zFile = zipOpen64(fileCompressed.toUtf8().constData(), APPEND_STATUS_CREATE);
    if (zFile == nullptr) {
        qCCritical(mcCore(), "cannot open zip file: %s", qUtf8Printable(fileCompressed));
        return false;
    }

    QMapIterator<QString, QStringList> itr(filePaths);
    while (itr.hasNext()) {
        auto item = itr.next();
        auto rootPath = item.key();
        auto files = item.value();
        QDir rootDir(rootPath);
        QString destPath;
        QFileInfo info;
        for (int index = 0; index < files.size(); ++index) {
            const QString &file(files.at(index));
            info.setFile(file);
            if (rootPath.isEmpty()) {
                destPath = info.fileName();
            } else {
                destPath = rootDir.relativeFilePath(info.absoluteFilePath());
            }
            if (!info.exists() || !compressFile(zFile, file, destPath)) {
                zipClose(zFile, nullptr);
                QFile::remove(fileCompressed);
                return false;
            }
        }
    }
    zipClose(zFile, nullptr);
    return true;
}
