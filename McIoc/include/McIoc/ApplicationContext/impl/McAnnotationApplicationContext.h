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

MC_FORWARD_DECL_PRIVATE_DATA(McAnnotationApplicationContext);

class MCIOC_EXPORT McAnnotationApplicationContext : public McReadableApplicationContext
{
    Q_OBJECT
    MC_DECL_INIT(McAnnotationApplicationContext)
public:
    McAnnotationApplicationContext(IMcConfigurableBeanFactoryConstPtrRef factory,
                                   IMcBeanDefinitionReaderConstPtrRef reader,
                                   QObject *parent = nullptr);
    McAnnotationApplicationContext(IMcBeanDefinitionReaderConstPtrRef reader,
                                   QObject *parent = nullptr);
    McAnnotationApplicationContext(QObject *parent = nullptr);

    ~McAnnotationApplicationContext() override;

    void generateReader() noexcept;

    static void addConnect(const QString &beanName,
                           const QString &sender,
                           const QString &signal,
                           const QString &receiver,
                           const QString &slot,
                           Qt::ConnectionType type) noexcept;
    
private:
    MC_DECL_PRIVATE(McAnnotationApplicationContext)
};

MC_DECL_POINTER(McAnnotationApplicationContext)
