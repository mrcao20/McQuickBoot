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
#include "McLog/Appender/impl/McByDayRollingFileAppender.h"

#include <QFile>

#include "McLog/Utils/McFileUtils.h"

MC_INIT(McByDayRollingFileAppender)
MC_REGISTER_BEAN_FACTORY(McByDayRollingFileAppender)
MC_INIT_END

MC_DECL_PRIVATE_DATA(McByDayRollingFileAppender)
int day{1};
MC_DECL_PRIVATE_DATA_END

McByDayRollingFileAppender::McByDayRollingFileAppender()
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

bool McByDayRollingFileAppender::isNewNextFile() noexcept
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
