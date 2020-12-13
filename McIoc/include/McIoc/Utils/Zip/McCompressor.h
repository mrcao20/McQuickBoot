#pragma once

#include "../../McMacroGlobal.h"

#include <QString>

class MCIOC_EXPORT McCompressor
{
public:
    static bool compressDir(QString fileCompressed, QString dir = QString()) noexcept;
    
    static bool compressFiles(QString fileCompressed, QStringList files) noexcept;
};
