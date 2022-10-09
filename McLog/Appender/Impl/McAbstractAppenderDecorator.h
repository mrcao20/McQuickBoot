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
