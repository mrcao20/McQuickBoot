#pragma once

#include <QString>

class McFileUtils
{
public:
    static bool judgeDate(const QString &filePath, int day) noexcept;
};
