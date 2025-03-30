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
#include "AnnotationApplicationContextTest.h"

#include <QtTest>

#include <McIoc/ApplicationContext/Impl/McAnnotationApplicationContext.h>

void AnnotationApplicationContextTest::initTestCase()
{
    mcRegisterMetaTypeSimple<SimpleGadget>();
    mcRegisterMetaTypeSimple<SimpleGadgetPointer>();
    mcRegisterMetaTypeSimple<ComplexGadget>();

    mcRegisterMetaType<SimpleObject>();
    mcRegisterMetaType<SimpleObject2>();
    mcRegisterMetaType<SimpleObjectPointer>();
    mcRegisterMetaType<SimpleObjectPointer2>();
    mcRegisterMetaTypeSimple<ComplexObject>();
    mcRegisterMetaTypeSimple<ObjectInterface>();
    mcRegisterMetaTypeSimple<ObjectPointerInterface>();
    mcRegisterContainer<QList<ObjectInterfacePtr>>();
    mcRegisterContainer<QMap<QString, ObjectInterfacePtr>>();
    mcRegisterContainer<QList<ObjectPointerInterface *>>();
    mcRegisterContainer<QMap<QString, ObjectPointerInterface *>>();

    m_appCtx = McAnnotationApplicationContextPtr::create();
    m_appCtx->refresh();
}

void AnnotationApplicationContextTest::gadgetCase()
{
    {
        QVERIFY(m_appCtx->containsBean("simpleGadget"));
        QVERIFY(!m_appCtx->isSingleton("simpleGadget"));
        QVERIFY(!m_appCtx->isPointer("simpleGadget"));
        auto bean = m_appCtx->getBean<SimpleGadgetPtr>("simpleGadget");
        QVERIFY(!bean.isNull());
        QVERIFY(bean->text == "simpleGadget");
    }
    {
        QVERIFY(!m_appCtx->containsBean("simpleGadgetPointer"));
        QVERIFY(m_appCtx->containsBean("customSimpleGadget"));
        QVERIFY(m_appCtx->isSingleton("customSimpleGadget"));
        QVERIFY(m_appCtx->isPointer("customSimpleGadget"));
        auto bean = m_appCtx->getBean<SimpleGadgetPointer *>("customSimpleGadget");
        QVERIFY(bean != nullptr);
        QVERIFY(bean->text == "simpleGadgetPointer");
    }
    {
        QVERIFY(m_appCtx->containsBean("complexGadget"));
        QVERIFY(m_appCtx->isSingleton("complexGadget"));
        QVERIFY(!m_appCtx->isPointer("complexGadget"));
        auto bean = m_appCtx->getBean<ComplexGadgetPtr>("complexGadget");
        QVERIFY(!bean.isNull());
        QVERIFY(!bean->simpleGadget.isNull());
        QVERIFY(!bean->simpleGadget2.isNull());
        //! 非单例，每次获取的对象均不一样
        QVERIFY(bean->simpleGadget != bean->simpleGadget2);
        QVERIFY(bean->simpleGadgetPointer != nullptr);
        QVERIFY(bean->simpleGadgetPointer2 != nullptr);
        QVERIFY(bean->simpleGadgetPointer == bean->simpleGadgetPointer2);
    }
}

void AnnotationApplicationContextTest::objectCase()
{
    {
        QVERIFY(m_appCtx->containsBean("simpleObject"));
        QVERIFY(!m_appCtx->isSingleton("simpleObject"));
        QVERIFY(!m_appCtx->isPointer("simpleObject"));
        auto bean = m_appCtx->getBean<SimpleObjectPtr>("simpleObject");
        QVERIFY(!bean.isNull());
        QVERIFY(bean->test() == "simpleObject");
        auto ibean = m_appCtx->getBean<ObjectInterfacePtr>("simpleObject");
        QVERIFY(!ibean.isNull());
        QVERIFY(ibean->test() == "simpleObject");
    }
    {
        QVERIFY(!m_appCtx->containsBean("simpleObjectPointer"));
        QVERIFY(m_appCtx->containsBean("customSimpleObject"));
        QVERIFY(m_appCtx->isSingleton("customSimpleObject"));
        QVERIFY(m_appCtx->isPointer("customSimpleObject"));
        auto bean = m_appCtx->getBean<SimpleObjectPointer *>("customSimpleObject");
        QVERIFY(bean != nullptr);
        QVERIFY(bean->test() == "simpleObjectPointer");
        auto ibean = m_appCtx->getBean<ObjectPointerInterface *>("customSimpleObject");
        QVERIFY(ibean != nullptr);
        QVERIFY(ibean->test() == "simpleObjectPointer");
    }
    {
        QVERIFY(m_appCtx->containsBean("complexObject"));
        QVERIFY(m_appCtx->isSingleton("complexObject"));
        QVERIFY(!m_appCtx->isPointer("complexObject"));
        auto bean = m_appCtx->getBean<ComplexObjectPtr>("complexObject");
        QVERIFY(!bean.isNull());
        QVERIFY(!bean->simpleObject.isNull());
        QVERIFY(!bean->simpleObject2.isNull());
        QVERIFY(bean->customSimpleObject != nullptr);
        QVERIFY(bean->customSimpleObject2 != nullptr);
        QVERIFY(bean->objectInterfaceList.size() == 2);
        QVERIFY(bean->objectInterfaceMap.size() == 2);
        QVERIFY(bean->objectInterfaceMap.contains("simpleObject"));
        QVERIFY(bean->objectInterfaceMap.contains("simpleObject2"));
        QVERIFY(bean->objectPointerInterfaceList.size() == 2);
        QVERIFY(bean->objectPointerInterfaceMap.size() == 2);
        QVERIFY(bean->objectPointerInterfaceMap.contains("customSimpleObject"));
        QVERIFY(bean->objectPointerInterfaceMap.contains("customSimpleObject2"));
    }
}
