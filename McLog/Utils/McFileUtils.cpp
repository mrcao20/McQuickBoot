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
