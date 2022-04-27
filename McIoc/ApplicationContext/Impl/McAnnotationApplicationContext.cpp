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
#include "McAnnotationApplicationContext.h"

#include <QMetaEnum>

#include "BeanBuilder/Impl/McBeanConnector.h"
#include "BeanBuilder/Impl/McBeanEnum.h"
#include "BeanBuilderReader/Impl/McAnnotationBeanDefinitionReader.h"

MC_GLOBAL_STATIC_BEGIN(staticData)
QMultiMap<QString, McBeanConnectorPtr> beanConnectors;
MC_GLOBAL_STATIC_END(staticData)

McAnnotationApplicationContext::McAnnotationApplicationContext() noexcept
{
    auto reader = McAnnotationBeanDefinitionReaderPtr::create(McMetaType::metaTypes(), staticData->beanConnectors);
    setReader(reader);
}

McAnnotationApplicationContext::McAnnotationApplicationContext(
    const IMcConfigurableBeanFactoryPtr &factory, const IMcBeanBuilderReaderPtr &reader) noexcept
    : McReadableApplicationContext(factory, reader)
{
}

McAnnotationApplicationContext::McAnnotationApplicationContext(const IMcBeanBuilderReaderPtr &reader) noexcept
    : McReadableApplicationContext(reader)
{
}

void McAnnotationApplicationContext::addConnect(const QString &beanName, const QString &sender, const QString &signal,
    const QString &receiver, const QString &slot, Qt::ConnectionType type) noexcept
{
    auto con = McBeanConnectorPtr::create();
    con->setSender(sender);
    con->setSignal(signal);
    con->setReceiver(receiver);
    con->setSlot(slot);
    auto e = McBeanEnumPtr::create();
    e->setScope(QLatin1String("Qt"));
    e->setType(QLatin1String("ConnectionType"));
    auto metaObj = &Qt::staticMetaObject;
    auto index = metaObj->indexOfEnumerator("ConnectionType");
    e->setValue(metaObj->enumerator(index).valueToKeys(type));
    con->setType(e);
    staticData->beanConnectors.insert(beanName, con);
}
