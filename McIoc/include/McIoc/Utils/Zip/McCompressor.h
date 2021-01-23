#pragma once

#include "../../McMacroGlobal.h"

#include <QString>

class QuaZip;

class MCIOC_EXPORT McCompressor
{
public:
    static bool compressDir(const QString &fileCompressed, const QString &dir = QString()) noexcept;

    static bool compressFiles(const QString &fileCompressed,
                              const QStringList &files,
                              const QString &rootPath = QString()) noexcept;
    static bool compressFiles(const QString &fileCompressed,
                              QMap<QString, QStringList> filePaths) noexcept;

private:
    static bool compressFile(QuaZip *zip, const QString &fileName, const QString &fileDest) noexcept;
};
