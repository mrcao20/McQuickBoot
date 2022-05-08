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
#include "McFileUtils.h"

#include <QDateTime>
#include <QFileInfo>

#include "../McLogGlobal.h"

bool McFileUtils::judgeDate(const QString &filePath, int day) noexcept
{
    QFileInfo fileInfo(filePath);
    if (!fileInfo.exists()) {
        return true;
    }
    auto dateTime = fileInfo.birthTime();
    if (!dateTime.isValid()) {
        dateTime = fileInfo.metadataChangeTime();
    }
    if (!dateTime.isValid()) {
        qCCritical(mcLog(), "failed get birth time of the file: %s\n", qUtf8Printable(filePath));
        return true;
    }
    auto curDateTime = QDateTime::currentDateTime();
    return qAbs(curDateTime.daysTo(dateTime)) >= day;
}
