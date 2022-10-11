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
#include "McAbstractAppenderDecorator.h"

#include <QTimer>

#include "McAppenderSeparator.h"

MC_AUTO_INIT(McAbstractAppenderDecorator)
mcRegisterMetaTypeSimple<McAppenderSeparator>();
mcRegisterContainer<QList<IMcConfigurableAppenderPtr>>();
MC_INIT_END

MC_DECL_PRIVATE_DATA(McAbstractAppenderDecorator)
McAppenderSeparatorPtr separator;
QList<IMcConfigurableAppenderPtr> appenders;
QString threshold;
QTimer autoSeparateTimer;
MC_DECL_PRIVATE_DATA_END

McAbstractAppenderDecorator::McAbstractAppenderDecorator() noexcept
{
    MC_NEW_PRIVATE_DATA(McAbstractAppenderDecorator);
}

McAbstractAppenderDecorator::~McAbstractAppenderDecorator() {}

McAppenderSeparatorPtr McAbstractAppenderDecorator::separator() const noexcept
{
    return d->separator;
}

void McAbstractAppenderDecorator::setSeparator(const McAppenderSeparatorPtr &val) noexcept
{
    d->separator = val;
}

QList<IMcConfigurableAppenderPtr> McAbstractAppenderDecorator::appenders() const noexcept
{
    return d->appenders;
}

void McAbstractAppenderDecorator::setAppenders(const QList<IMcConfigurableAppenderPtr> &val) noexcept
{
    d->appenders = val;
}

QString McAbstractAppenderDecorator::threshold() const noexcept
{
    return d->threshold;
}

void McAbstractAppenderDecorator::setThreshold(const QString &val) noexcept
{
    d->threshold = val;

    for (auto &appender : qAsConst(d->appenders)) {
        if (appender->threshold().isEmpty()) {
            appender->setThreshold(threshold());
        }
    }
}

QList<QtMsgType> McAbstractAppenderDecorator::types() const noexcept
{
    return QList<QtMsgType>();
}

void McAbstractAppenderDecorator::append(QtMsgType type, const QMessageLogContext &context, const QString &str) noexcept
{
    for (auto &appender : qAsConst(d->appenders)) {
        doAppend(appender, type, context, str);
    }
}

void McAbstractAppenderDecorator::buildFinished() noexcept
{
    if (d->separator.isNull()) {
        d->separator = McAppenderSeparatorPtr::create();
    }
    if (d->separator->isAutoSeparate) {
        //! 回归到本类线程中执行
        connect(
            &d->autoSeparateTimer, &QTimer::timeout, this,
            [this]() {
                QMessageLogContext context(QT_MESSAGELOG_FILE, QT_MESSAGELOG_LINE, QT_MESSAGELOG_FUNC, nullptr);
                for (auto &appender : qAsConst(d->appenders)) {
                    auto types = appender->types();
                    auto type = types.first(); //!< 不可能为空
                    for (auto &t : qAsConst(types)) {
                        if (t <= type) {
                            continue;
                        }
                        type = t;
                    }
                    writeSeparator(appender, type, context);
                }
            },
            Qt::QueuedConnection);
        d->autoSeparateTimer.start(separator()->autoSeparateInterval);
    }
}

void McAbstractAppenderDecorator::writeSeparator(
    const IMcConfigurableAppenderPtr &appender, QtMsgType type, const QMessageLogContext &context) noexcept
{
    appender->append(type, context, separator()->separator);
}
