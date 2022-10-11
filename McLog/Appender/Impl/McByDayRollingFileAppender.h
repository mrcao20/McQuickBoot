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
#pragma once

#include "McRollingFileAppender.h"

MC_FORWARD_DECL_PRIVATE_DATA(McByDayRollingFileAppender)

class MC_LOG_EXPORT McByDayRollingFileAppender : public McRollingFileAppender
{
    Q_OBJECT
    MC_FULL_DEFINE(McByDayRollingFileAppender, McRollingFileAppender)
    Q_PROPERTY(int day READ day WRITE setDay)
public:
    McByDayRollingFileAppender() noexcept;
    ~McByDayRollingFileAppender();

    int day() const noexcept;
    void setDay(int val) noexcept;

protected:
    bool isRolling() noexcept override;

private:
    MC_DECL_PRIVATE(McByDayRollingFileAppender)
};

MC_DECL_METATYPE(McByDayRollingFileAppender)
