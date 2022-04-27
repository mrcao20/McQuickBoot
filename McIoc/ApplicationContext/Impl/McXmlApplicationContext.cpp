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
#include "McXmlApplicationContext.h"

#include "BeanBuilderReader/Impl/McXmlBeanBuilderReader.h"

McXmlApplicationContext::McXmlApplicationContext() noexcept {}

McXmlApplicationContext::McXmlApplicationContext(const QIODevicePtr &device, const QString &flag) noexcept
    : McXmlApplicationContext(QList<QIODevicePtr>() << device, flag)
{
}

McXmlApplicationContext::McXmlApplicationContext(const QList<QIODevicePtr> &devices, const QString &flag) noexcept
{
    setDevices(devices, flag);
}

McXmlApplicationContext::McXmlApplicationContext(const IMcBeanBuilderReaderPtr &reader) noexcept
    : McReadableApplicationContext(reader)
{
}

McXmlApplicationContext::McXmlApplicationContext(
    const IMcConfigurableBeanFactoryPtr &factory, const IMcBeanBuilderReaderPtr &reader) noexcept
    : McReadableApplicationContext(factory, reader)
{
}

void McXmlApplicationContext::setDevice(const QIODevicePtr &device, const QString &flag) noexcept
{
    setDevices(QList<QIODevicePtr>() << device, flag);
}

void McXmlApplicationContext::setDevices(const QList<QIODevicePtr> &devices, const QString &flag) noexcept
{
    setReader(McXmlBeanBuilderReaderPtr::create(devices, flag));
}
