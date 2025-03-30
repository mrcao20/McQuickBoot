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
#include "XmlBuilder.h"

#include <QBuffer>

#include <McIoc/ApplicationContext/Impl/McXmlApplicationContext.h>
#include <McIoc/Utils/XmlBuilder/Impl/McBeanCollection.h>
#include <McIoc/Utils/XmlBuilder/Impl/McClassBean.h>
#include <McIoc/Utils/XmlBuilder/Impl/McConnect.h>
#include <McIoc/Utils/XmlBuilder/Impl/McConstructorArg.h>
#include <McIoc/Utils/XmlBuilder/Impl/McEnum.h>
#include <McIoc/Utils/XmlBuilder/Impl/McList.h>
#include <McIoc/Utils/XmlBuilder/Impl/McMap.h>
#include <McIoc/Utils/XmlBuilder/Impl/McPlaceholder.h>
#include <McIoc/Utils/XmlBuilder/Impl/McPluginBean.h>
#include <McIoc/Utils/XmlBuilder/Impl/McPluginChecker.h>
#include <McIoc/Utils/XmlBuilder/Impl/McProperty.h>
#include <McIoc/Utils/XmlBuilder/Impl/McRef.h>
#include <McIoc/Utils/XmlBuilder/Impl/McValue.h>

IMcApplicationContextPtr XmlBuilder::build()
{
    auto buffer = QSharedPointer<QBuffer>::create();
    bool flag = buffer->open(QIODevice::ReadWrite);
    Q_ASSERT(flag);
    Mc::XmlBuilder::BeanCollection beans;
    beans.addBeans(buildPod());
    beans.addBeans(buildGadget());
    beans.addBeans(buildContainer());
    beans.addBeans(buildObject());
    beans.writeToDevice(buffer.data(), -1);
    return McXmlApplicationContextPtr::create(buffer);
}

QList<Mc::XmlBuilder::IBeanPtr> XmlBuilder::buildPod()
{
    using namespace Mc::XmlBuilder;
    QList<Mc::XmlBuilder::IBeanPtr> beans;
    {
        auto bean = ClassBeanPtr::create("PodTest", "podTest");
        beans.append(bean);
    }
    {
        auto bean = ClassBeanPtr::create("PodTest", "podTestPointer");
        bean->setPointer(true);
        beans.append(bean);
    }
    return beans;
}

QList<Mc::XmlBuilder::IBeanPtr> XmlBuilder::buildGadget()
{
    using namespace Mc::XmlBuilder;
    QList<Mc::XmlBuilder::IBeanPtr> beans;
    {
        auto bean = ClassBeanPtr::create("GadgetTest", "gadgetTest");
        auto pro = PropertyPtr::create("text", "gadgetTest1");
        bean->addContent(pro);
        beans.append(bean);
    }
    {
        auto bean = ClassBeanPtr::create("GadgetTest", "gadgetTestPointer");
        bean->setPointer(true);
        auto con = ConstructorArgPtr::create();
        con->setContent("gadgetTest2");
        bean->addContent(con);
        beans.append(bean);
    }
    return beans;
}

QList<Mc::XmlBuilder::IBeanPtr> XmlBuilder::buildContainer()
{
    using namespace Mc::XmlBuilder;
    QList<Mc::XmlBuilder::IBeanPtr> beans;
    {
        auto bean = ClassBeanPtr::create("List", "listTest");
        auto list = ListPtr::create();
        list->addContent(QLatin1String("停封"));
        list->addContent(QLatin1String("薄纸"));
        list->addContent(QLatin1String("关系"));
        bean->addContent(list);
        beans.append(bean);
    }
    {
        auto bean = ClassBeanPtr::create("Map", "mapTest");
        auto map = MapPtr::create();
        map->addContent(QLatin1String("jack"), QLatin1String("杰克"));
        map->addContent(QLatin1String("rose"), QLatin1String("肉丝"));
        bean->addContent(map);
        beans.append(bean);
    }
    return beans;
}

QList<Mc::XmlBuilder::IBeanPtr> XmlBuilder::buildObject()
{
    using namespace Mc::XmlBuilder;
    QList<Mc::XmlBuilder::IBeanPtr> beans;
    {
        auto bean = ClassBeanPtr::create("ObjectTest", "objectTest");
        bean->setSingleton(false);
        bean->setParent("objectTestPointer");
        bean->addContent(ConstructorArgPtr::create(0, "objectTest1"));
        {
            auto child = ClassBeanPtr::create("ObjectTest");
            child->addContent(PropertyPtr::create("text", "innerObjectTest"));
            bean->addContent(PropertyPtr::create("object", child));
        }
        beans.append(bean);
    }
    {
        auto bean = ClassBeanPtr::create("ObjectTest", "objectTestPointer");
        bean->setPointer(true);
        bean->addContent(ConstructorArgPtr::create("val", "objectTestPointer"));
        bean->addContent(PropertyPtr::create("object", RefPtr::create("objectTest")));
        auto con = ConnectPtr::create();
        con->setSender("this");
        con->setSignal("testSignal()");
        con->setReceiver("object");
        con->setSlot("testSlot()");
        con->setConnectionType("DirectConnection | UniqueConnection");
        bean->addContent(con);
        bean->addContent(PropertyPtr::create("list", RefPtr::create("listTest")));
        auto map = MapPtr::create();
        map->addContent("jack", RefPtr::create("objectTest"));
        bean->addContent(PropertyPtr::create("map", map));
        beans.append(bean);
    }
    return beans;
}
