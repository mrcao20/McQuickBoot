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
#include "McCompressor.h"

#include <QDir>
#include <QFile>
#include <QMap>
#include <QScopeGuard>
#include <QtGlobal>

#include "../McGlobal.h"
#include "zlib/zip.h"

static bool compressFile(zipFile zFile, const QString &filePath, const QString &destPath) noexcept
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
