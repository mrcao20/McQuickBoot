#include "McIoc/Utils/Zip/McCompressor.h"

#include "JlCompress.h"

bool McCompressor::compressDir(QString fileCompressed, QString dir) noexcept
{
    return JlCompress::compressDir(fileCompressed, dir);
}

bool McCompressor::compressFiles(QString fileCompressed, QStringList files) noexcept
{
    return JlCompress::compressFiles(fileCompressed, files);
}
