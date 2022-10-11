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

#include "../IMcLogger.h"

#ifdef MC_USE_QT6
Q_MOC_INCLUDE("Appender/IMcConfigurableAppender.h")
#endif

MC_FORWARD_DECL_CLASS(IMcConfigurableAppender)

MC_FORWARD_DECL_PRIVATE_DATA(McLogger)

class MC_LOG_EXPORT McLogger
    : public QObject
    , public IMcLogger
    , public IMcBeanBuildable
{
    Q_OBJECT
    MC_FULL_DEFINE(McLogger, QObject, IMcLogger)
    Q_PROPERTY(QString threshold READ threshold WRITE setThreshold)
    Q_PROPERTY(QList<IMcConfigurableAppenderPtr> appenders READ appenders WRITE setAppenders)
public:
    McLogger() noexcept;
    ~McLogger();

    QString threshold() const noexcept;
    void setThreshold(const QString &val) noexcept;

    QList<IMcConfigurableAppenderPtr> appenders() const noexcept;
    void setAppenders(const QList<IMcConfigurableAppenderPtr> &val) noexcept;

    void log(QtMsgType type, const QMessageLogContext &context, const QString &str) noexcept override;

    void buildFinished() noexcept override;

private:
    MC_DECL_PRIVATE(McLogger)
};

MC_DECL_METATYPE(McLogger)
