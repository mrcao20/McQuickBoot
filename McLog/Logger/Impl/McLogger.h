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
#pragma once

#include <QList>

#include <McIoc/Utils/IMcBeanBuildable.h>

#include "../IMcLogger.h"

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
