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
#include "McByDayRollingFileAppender.h"

#include <QFile>

#include "Utils/McFileUtils.h"

MC_AUTO_INIT(McByDayRollingFileAppender)
MC_INIT_END

MC_DECL_PRIVATE_DATA(McByDayRollingFileAppender)
int day{1};
MC_DECL_PRIVATE_DATA_END

McByDayRollingFileAppender::McByDayRollingFileAppender() noexcept
{
    MC_NEW_PRIVATE_DATA(McByDayRollingFileAppender);
}

McByDayRollingFileAppender::~McByDayRollingFileAppender() {}

int McByDayRollingFileAppender::day() const noexcept
{
    return d->day;
}

void McByDayRollingFileAppender::setDay(int val) noexcept
{
    d->day = val;
}

bool McByDayRollingFileAppender::isRolling() noexcept
{
    if (device().isNull()) {
        return false;
    }
    auto file = device().staticCast<QFile>();
    if (!file->isOpen()) {
        return false;
    }
    auto filePath = file->fileName();
    return McFileUtils::judgeDate(filePath, d->day);
}
