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
#include "McLog/Appender/Decorator/McAbstractAppenderDecorator.h"

#include <QTimer>

#include "McLog/Appender/Decorator/McAppenderSeparator.h"

MC_INIT(McAbstractAppenderDecorator)
MC_REGISTER_BEAN_FACTORY(McAppenderSeparator)
MC_REGISTER_BEAN_FACTORY(McAbstractAppenderDecorator)
MC_REGISTER_LIST_CONVERTER(QList<IMcConfigurableAppenderPtr>)
MC_INIT_END

MC_DECL_PRIVATE_DATA(McAbstractAppenderDecorator)
McAppenderSeparatorPtr separator;
QString threshold;
QList<IMcConfigurableAppenderPtr> appenders;
QTimer autoSeparateTimer;
MC_DECL_PRIVATE_DATA_END

McAbstractAppenderDecorator::McAbstractAppenderDecorator(QObject *parent)
    : QObject(parent)
{
    MC_NEW_PRIVATE_DATA(McAbstractAppenderDecorator);
}

McAbstractAppenderDecorator::~McAbstractAppenderDecorator()
{
}

McAppenderSeparatorPtr McAbstractAppenderDecorator::separator() const noexcept
{
    return d->separator;
}

void McAbstractAppenderDecorator::setSeparator(const McAppenderSeparatorPtr &val) noexcept
{
    d->separator = val;
}

QString McAbstractAppenderDecorator::threshold() const noexcept
{
    return d->threshold;
}

void McAbstractAppenderDecorator::setThreshold(const QString &val) noexcept
{
    d->threshold = val;
    
    for(auto appender : d->appenders) {
        if(appender->threshold().isEmpty()) {
            appender->setThreshold(threshold());
        }
    }
}

QList<QtMsgType> McAbstractAppenderDecorator::types() const noexcept
{
    return QList<QtMsgType>();
}

QList<IMcConfigurableAppenderPtr> McAbstractAppenderDecorator::appenders() const noexcept
{
    return d->appenders;
}

void McAbstractAppenderDecorator::setAppenders(const QList<IMcConfigurableAppenderPtr> &val) noexcept
{
    d->appenders = val;
}

void McAbstractAppenderDecorator::append(QtMsgType type, const QMessageLogContext &context, const QString &str) noexcept
{
    for(auto appender : appenders()) {
        doAppend(appender, type, context, str);
    }
}

void McAbstractAppenderDecorator::finished() noexcept
{
    //! 回归到本类线程中执行
    connect(&d->autoSeparateTimer, &QTimer::timeout, this, [this](){
        QMessageLogContext context(QT_MESSAGELOG_FILE, QT_MESSAGELOG_LINE, QT_MESSAGELOG_FUNC, nullptr);
        for(auto appender : appenders()) {
            auto types = appender->types();
            auto type = types.first();      //!< 不可能为空
            for(auto t : types) {
                if(t <= type) {
                    continue;
                }
                type = t;
            }
            writeSeparator(appender, type, context);
        }
    }, Qt::QueuedConnection);
    d->autoSeparateTimer.start(separator()->autoSeparateInterval);
}

void McAbstractAppenderDecorator::writeSeparator(IMcConfigurableAppenderConstPtrRef appender, QtMsgType type, const QMessageLogContext &context) noexcept
{
    appender->append(type, context, separator()->separator);
}
