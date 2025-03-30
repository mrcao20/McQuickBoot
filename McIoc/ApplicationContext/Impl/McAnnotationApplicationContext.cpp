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
    e->setScope(QStringLiteral("Qt"));
    e->setType(QStringLiteral("ConnectionType"));
#ifdef MC_USE_QT5
    auto metaObj = qt_getQtMetaObject();
#else
    auto metaObj = &Qt::staticMetaObject;
#endif
    auto index = metaObj->indexOfEnumerator("ConnectionType");
    e->setValue(metaObj->enumerator(index).valueToKeys(type));
    con->setType(e);
    staticData->beanConnectors.insert(beanName, con);
}
