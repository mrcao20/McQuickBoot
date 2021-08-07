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

#include "McAbstractWidgetApplicationContext.h"

QT_BEGIN_NAMESPACE
MC_FORWARD_DECL_CLASS(QIODevice);
QT_END_NAMESPACE

MC_FORWARD_DECL_PRIVATE_DATA(McXmlWidgetApplicationContext);

class MCWIDGETIOC_EXPORT McXmlWidgetApplicationContext : public McAbstractWidgetApplicationContext
{
    Q_OBJECT
public:
    explicit McXmlWidgetApplicationContext(QObject *parent = nullptr);
    McXmlWidgetApplicationContext(QIODeviceConstPtrRef device,
                                  const QString &flag = QString(),
                                  QObject *parent = nullptr);
    McXmlWidgetApplicationContext(const QList<QIODevicePtr> &devices,
                                  const QString &flag = QString(),
                                  QObject *parent = nullptr);
    McXmlWidgetApplicationContext(const QString &location,
                                  const QString &flag = QString(),
                                  QObject *parent = nullptr);
    McXmlWidgetApplicationContext(const QStringList &locations,
                                  const QString &flag = QString(),
                                  QObject *parent = nullptr);
    ~McXmlWidgetApplicationContext() override;

    void setDevices(const QList<QIODevicePtr> &devices, const QString &flag = QString()) noexcept;

private:
    MC_DECL_PRIVATE(McXmlWidgetApplicationContext)
};

MC_DECL_POINTER(McXmlWidgetApplicationContext)
