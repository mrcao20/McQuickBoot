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
#include "McLog/Logger/impl/McLogger.h"

#include "McLog/Appender/IMcConfigurableAppender.h"

MC_DECL_PRIVATE_DATA(McLogger)
QString threshold;
QList<IMcConfigurableAppenderPtr> appenders;
MC_DECL_PRIVATE_DATA_END

MC_INIT(McLogger)
MC_REGISTER_BEAN_FACTORY(McLogger)
MC_REGISTER_LIST_CONVERTER(QList<IMcConfigurableAppenderPtr>)
MC_INIT_END

McLogger::McLogger() 
{
    MC_NEW_PRIVATE_DATA(McLogger);
}

McLogger::~McLogger()
{
}

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

void McLogger::finished() noexcept
{
    if(threshold().isEmpty())
        setThreshold("debug-");
    for(auto appender : d->appenders) {
        if(appender->threshold().isEmpty()) {
            appender->setThreshold(threshold());
        }
    }
}

void McLogger::log(QtMsgType type, const QMessageLogContext &context, const QString &str) noexcept
{
    for(auto appender : d->appenders) {
        appender->append(type, context, str);
    }
}
