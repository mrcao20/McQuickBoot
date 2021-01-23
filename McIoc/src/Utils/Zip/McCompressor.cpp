#include "McIoc/Utils/Zip/McCompressor.h"

#include "JlCompress.h"

namespace {

static bool copyData(QIODevice &inFile, QIODevice &outFile)
{
    while (!inFile.atEnd()) {
        char buf[4096];
        qint64 readLen = inFile.read(buf, 4096);
        if (readLen <= 0)
            return false;
        if (outFile.write(buf, readLen) != readLen)
            return false;
    }
    return true;
}

} // namespace

bool McCompressor::compressDir(const QString &fileCompressed, const QString &dir) noexcept
{
    return JlCompress::compressDir(fileCompressed, dir);
}

bool McCompressor::compressFiles(const QString &fileCompressed,
                                 const QStringList &files,
                                 const QString &rootPath) noexcept
{
    QMap<QString, QStringList> filePaths;
    filePaths.insert(rootPath, files);
    return compressFiles(fileCompressed, filePaths);
}

bool McCompressor::compressFiles(const QString &fileCompressed,
                                 QMap<QString, QStringList> filePaths) noexcept
{
    // Creo lo zip
    QuaZip zip(fileCompressed);
    QDir().mkpath(QFileInfo(fileCompressed).absolutePath());
    if (!zip.open(QuaZip::mdCreate)) {
        QFile::remove(fileCompressed);
        return false;
    }

    auto rootPaths = filePaths.keys();
    for (auto rootPath : rootPaths) {
        auto files = filePaths.value(rootPath);
        // Comprimo i file
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
            if (!info.exists() || !compressFile(&zip, file, destPath)) {
                QFile::remove(fileCompressed);
                return false;
            }
        }

        // Chiudo il file zip
        zip.close();
        if (zip.getZipError() != 0) {
            QFile::remove(fileCompressed);
            return false;
        }
    }

    return true;
}

bool McCompressor::compressFile(QuaZip *zip,
                                const QString &fileName,
                                const QString &fileDest) noexcept
{
    // zip: oggetto dove aggiungere il file
    // fileName: nome del file reale
    // fileDest: nome del file all'interno del file compresso

    // Controllo l'apertura dello zip
    if (!zip)
        return false;
    if (zip->getMode() != QuaZip::mdCreate && zip->getMode() != QuaZip::mdAppend
        && zip->getMode() != QuaZip::mdAdd)
        return false;

    // Apro il file risulato
    QuaZipFile outFile(zip);
    if (!outFile.open(QIODevice::WriteOnly, QuaZipNewInfo(fileDest, fileName)))
        return false;

    QFileInfo input(fileName);
    if (quazip_is_symlink(input)) {
        // Not sure if we should use any specialized codecs here.
        // After all, a symlink IS just a byte array. And
        // this is mostly for Linux, where UTF-8 is ubiquitous these days.
        QString path = quazip_symlink_target(input);
        QString relativePath = input.dir().relativeFilePath(path);
        outFile.write(QFile::encodeName(relativePath));
    } else {
        QFile inFile;
        inFile.setFileName(fileName);
        if (!inFile.open(QIODevice::ReadOnly))
            return false;
        if (!copyData(inFile, outFile) || outFile.getZipError() != UNZ_OK)
            return false;
        inFile.close();
    }

    // Chiudo i file
    outFile.close();
    if (outFile.getZipError() != UNZ_OK)
        return false;

    return true;
}
