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

#include "../McLogGlobal.h"

MC_FORWARD_DECL_CLASS(IMcLogger)

class IMcLoggerRepository
{
    MC_DEFINE_INTERFACE(IMcLoggerRepository)
public:
    virtual QMap<QString, IMcLoggerPtr> loggers() const noexcept = 0;
    virtual void addLogger(const QString &loggerName, const IMcLoggerPtr &logger) noexcept = 0;
    virtual void setLogger(const QMap<QString, IMcLoggerPtr> &loggers) noexcept = 0;
    virtual IMcLoggerPtr getLogger(const QString &loggerName) noexcept = 0;
    virtual void runTask() noexcept = 0;
    virtual void flushWhenQuit() noexcept = 0;
};

MC_DECL_METATYPE(IMcLoggerRepository)
