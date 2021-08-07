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

#include "McReadableApplicationContext.h"

QT_BEGIN_NAMESPACE
MC_FORWARD_DECL_CLASS(QIODevice);
QT_END_NAMESPACE

MC_FORWARD_DECL_CLASS(IMcBeanDefinitionReader);

class MCIOC_EXPORT McXmlApplicationContext : public McReadableApplicationContext
{
    
    Q_OBJECT
public:
    explicit McXmlApplicationContext(QObject *parent = nullptr);
    McXmlApplicationContext(QIODeviceConstPtrRef device,
                            const QString &flag = QString(),
                            QObject *parent = nullptr);
    McXmlApplicationContext(const QList<QIODevicePtr> &devices,
                            const QString &flag = QString(),
                            QObject *parent = nullptr);
    McXmlApplicationContext(IMcBeanDefinitionReaderConstPtrRef reader,
                            QObject *parent = nullptr);
    McXmlApplicationContext(IMcConfigurableBeanFactoryConstPtrRef factory,
                            IMcBeanDefinitionReaderConstPtrRef reader,
                            QObject *parent = nullptr);
    ~McXmlApplicationContext() override;

    void setDevice(QIODeviceConstPtrRef device, const QString &flag = QString()) noexcept;
    void setDevices(const QList<QIODevicePtr> &devices, const QString &flag = QString()) noexcept;
};

MC_DECL_POINTER(McXmlApplicationContext)
