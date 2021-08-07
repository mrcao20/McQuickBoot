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
#include "McIoc/ApplicationContext/impl/McXmlApplicationContext.h"

#include <QIODevice>

#include "McIoc/BeanDefinitionReader/impl/McXmlBeanDefinitionReader.h"
#include "McIoc/PropertyParser/impl/McDefaultPropertyParser.h"

McXmlApplicationContext::McXmlApplicationContext(QObject *parent)
    : McReadableApplicationContext(parent)
{
}

McXmlApplicationContext::McXmlApplicationContext(QIODeviceConstPtrRef device,
                                                 const QString &flag,
                                                 QObject *parent)
    : McXmlApplicationContext(QList<QIODevicePtr>() << device, flag, parent)
{
}

McXmlApplicationContext::McXmlApplicationContext(const QList<QIODevicePtr> &devices,
                                                 const QString &flag,
                                                 QObject *parent)
    : McXmlApplicationContext(parent)
{
    setDevices(devices, flag);
}

McXmlApplicationContext::McXmlApplicationContext(IMcBeanDefinitionReaderConstPtrRef reader,
                                                 QObject *parent)
    : McReadableApplicationContext(reader, parent)
{
}

McXmlApplicationContext::McXmlApplicationContext(IMcConfigurableBeanFactoryConstPtrRef factory,
                                                 IMcBeanDefinitionReaderConstPtrRef reader,
                                                 QObject *parent)
    : McReadableApplicationContext(factory, reader, parent)
{
}

McXmlApplicationContext::~McXmlApplicationContext()
{
}

void McXmlApplicationContext::setDevice(QIODeviceConstPtrRef device, const QString &flag) noexcept
{
    setDevices(QList<QIODevicePtr>() << device, flag);
}

void McXmlApplicationContext::setDevices(const QList<QIODevicePtr> &devices,
                                         const QString &flag) noexcept
{
    setReader(
        McXmlBeanDefinitionReaderPtr::create(McDefaultPropertyParserPtr::create(), devices, flag));
}
