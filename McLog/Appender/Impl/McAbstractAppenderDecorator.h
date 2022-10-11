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

#include <QList>

#include <McIoc/Utils/IMcBeanBuildable.h>

#include "../IMcConfigurableAppender.h"

#ifdef MC_USE_QT6
Q_MOC_INCLUDE("Appender/Impl/McAppenderSeparator.h")
#endif

MC_FORWARD_DECL_STRUCT(McAppenderSeparator)

MC_FORWARD_DECL_PRIVATE_DATA(McAbstractAppenderDecorator)

class MC_LOG_EXPORT McAbstractAppenderDecorator
    : public QObject
    , public IMcConfigurableAppender
    , public IMcBeanBuildable
{
    Q_OBJECT
    MC_FULL_DEFINE(McAbstractAppenderDecorator, QObject, IMcConfigurableAppender)
    Q_PROPERTY(McAppenderSeparatorPtr separator READ separator WRITE setSeparator)
    Q_PROPERTY(QList<IMcConfigurableAppenderPtr> appenders READ appenders WRITE setAppenders)
public:
    McAbstractAppenderDecorator() noexcept;
    ~McAbstractAppenderDecorator();

    McAppenderSeparatorPtr separator() const noexcept;
    void setSeparator(const McAppenderSeparatorPtr &val) noexcept;

    QList<IMcConfigurableAppenderPtr> appenders() const noexcept;
    void setAppenders(const QList<IMcConfigurableAppenderPtr> &val) noexcept;

    QString threshold() const noexcept override;
    void setThreshold(const QString &val) noexcept override;

    QList<QtMsgType> types() const noexcept override;

    void append(QtMsgType type, const QMessageLogContext &context, const QString &str) noexcept override;

    void buildFinished() noexcept override;

protected:
    virtual void doAppend(const IMcConfigurableAppenderPtr &appender, QtMsgType type, const QMessageLogContext &context,
        const QString &str) noexcept = 0;

    void writeSeparator(
        const IMcConfigurableAppenderPtr &appender, QtMsgType type, const QMessageLogContext &context) noexcept;

private:
    MC_DECL_PRIVATE(McAbstractAppenderDecorator)
};

MC_DECL_POINTER(McAbstractAppenderDecorator)
