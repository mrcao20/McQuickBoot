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
#include "McLogger.h"

#include "Appender/IMcConfigurableAppender.h"

MC_DECL_PRIVATE_DATA(McLogger)
QString threshold;
QList<IMcConfigurableAppenderPtr> appenders;
MC_DECL_PRIVATE_DATA_END

MC_AUTO_INIT(McLogger)
mcRegisterContainer<QList<IMcConfigurableAppenderPtr>>();
MC_INIT_END

McLogger::McLogger() noexcept
{
    MC_NEW_PRIVATE_DATA(McLogger);
}

McLogger::~McLogger() {}

QString McLogger::threshold() const noexcept
{
    return d->threshold;
}

void McLogger::setThreshold(const QString &val) noexcept
{
    d->threshold = val;
}

QList<IMcConfigurableAppenderPtr> McLogger::appenders() const noexcept
{
    return d->appenders;
}

void McLogger::setAppenders(const QList<IMcConfigurableAppenderPtr> &val) noexcept
{
    d->appenders = val;
}

void McLogger::log(QtMsgType type, const QMessageLogContext &context, const QString &str) noexcept
{
    for (auto &appender : qAsConst(d->appenders)) {
        appender->append(type, context, str);
    }
}

void McLogger::buildFinished() noexcept
{
    if (threshold().isEmpty())
        setThreshold("debug-");
    for (auto &appender : qAsConst(d->appenders)) {
        if (appender->threshold().isEmpty()) {
            appender->setThreshold(threshold());
        }
    }
}
