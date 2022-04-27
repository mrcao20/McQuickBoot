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
    mcRegisterContainerConverter<QList<ObjectInterfacePtr>>();
    mcRegisterContainerConverter<QMap<QString, ObjectInterfacePtr>>();
    mcRegisterContainerConverter<QList<ObjectPointerInterface *>>();
    mcRegisterContainerConverter<QMap<QString, ObjectPointerInterface *>>();

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
