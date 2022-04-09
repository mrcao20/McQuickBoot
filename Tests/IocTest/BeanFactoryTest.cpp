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
#include "BeanFactoryTest.h"

#include <QTest>

#include <McIoc/BeanBuilder/Impl/McListBeanBuilder.h>
#include <McIoc/BeanBuilder/Impl/McMapBeanBuilder.h>
#include <McIoc/BeanBuilder/Impl/McSharedGadgetBeanBuilder.h>
#include <McIoc/BeanBuilder/Impl/McSharedObjectClassBeanBuilder.h>
#include <McIoc/BeanBuilder/Impl/McSharedObjectPluginBeanBuilder.h>
#include <McIoc/BeanFactory/Impl/McDefaultBeanFactory.h>

MC_AUTO_INIT(ObjectTest)
mcRegisterContainerConverter<QList<ObjectTestPtr>>();
mcRegisterContainerConverter<QMap<QString, ObjectTestPtr>>();
MC_INIT_END

ObjectTest::ObjectTest() {}

ObjectTest::ObjectTest(const QString &val) : m_text(val) {}

QString ObjectTest::test()
{
    return m_text;
}

void BeanFactoryTest::gadgetTestCase()
{
    mcRegisterMetaTypeSimple<PodTest>();
    mcRegisterMetaTypeSimple<GadgetTest>();
    auto beanFactory = McDefaultBeanFactoryPtr::create();
    {
        auto builder = McGadgetBeanBuilderPtr::create();
        builder->setReferenceResolver(beanFactory.data());
        builder->setClassName("PodTest");
        beanFactory->registerBeanBuilder("podTest", builder);
    }
    {
        auto builder = McGadgetBeanBuilderPtr::create();
        builder->setReferenceResolver(beanFactory.data());
        builder->setClassName("GadgetTest");
        beanFactory->registerBeanBuilder("gadgetTest", builder);
    }
    {
        auto builder = McGadgetBeanBuilderPtr::create();
        builder->setReferenceResolver(beanFactory.data());
        builder->setClassName("GadgetTest");
        builder->addProperty("text", "gadgetTestPro");
        beanFactory->registerBeanBuilder("gadgetTestPro", builder);
    }
    {
        auto builder = McGadgetBeanBuilderPtr::create();
        builder->setReferenceResolver(beanFactory.data());
        builder->setClassName("GadgetTest");
        builder->addConstructorArg(0, "gadgetTestConstructor");
        beanFactory->registerBeanBuilder("gadgetTestConstructor", builder);
    }
    /********************************************************/
    {
        auto podTest = beanFactory->getBeanPointer<PodTest>("podTest");
        QVERIFY(podTest != nullptr);
        QVERIFY(podTest->text == "podTest");
    }
    {
        auto gadgetTest = beanFactory->getBeanPointer<GadgetTest>("gadgetTest");
        QVERIFY(gadgetTest != nullptr);
        QVERIFY(gadgetTest->text == "gadgetTest");
    }
    {
        auto gadgetTest = beanFactory->getBeanPointer<GadgetTest>("gadgetTestPro");
        QVERIFY(gadgetTest != nullptr);
        QVERIFY(gadgetTest->text == "gadgetTestPro");
    }
    {
        mcRegisterMetaType<GadgetTest>();
        auto gadgetTest = beanFactory->getBeanPointer<GadgetTest>("gadgetTestConstructor");
        QVERIFY(gadgetTest != nullptr);
        QVERIFY(gadgetTest->text == "gadgetTestConstructor");
    }
}

void BeanFactoryTest::sharedGadgetTestCase()
{
    auto beanFactory = McDefaultBeanFactoryPtr::create();
    {
        auto builder = McSharedGadgetBeanBuilderPtr::create();
        builder->setReferenceResolver(beanFactory.data());
        builder->setClassName("PodTest");
        beanFactory->registerBeanBuilder("podTestShared", builder);
    }
    {
        auto builder = McSharedGadgetBeanBuilderPtr::create();
        builder->setReferenceResolver(beanFactory.data());
        builder->setClassName("GadgetTest");
        beanFactory->registerBeanBuilder("gadgetTestShared", builder);
    }
    {
        auto builder = McSharedGadgetBeanBuilderPtr::create();
        builder->setReferenceResolver(beanFactory.data());
        builder->setClassName("GadgetTest");
        builder->addProperty("text", "gadgetTestSharedPro");
        beanFactory->registerBeanBuilder("gadgetTestSharedPro", builder);
    }
    {
        auto builder = McSharedGadgetBeanBuilderPtr::create();
        builder->setReferenceResolver(beanFactory.data());
        builder->setClassName("GadgetTest");
        builder->addConstructorArg("val", "gadgetTestSharedConstructor");
        beanFactory->registerBeanBuilder("gadgetTestSharedConstructor", builder);
    }
    /********************************************************/
    {
        auto podTest = beanFactory->getBean<PodTest>("podTestShared");
        QVERIFY(podTest != nullptr);
        QVERIFY(podTest->text == "podTest");
    }
    {
        auto gadgetTest = beanFactory->getBean<GadgetTest>("gadgetTestShared");
        QVERIFY(gadgetTest != nullptr);
        QVERIFY(gadgetTest->text == "gadgetTest");
    }
    {
        auto gadgetTest = beanFactory->getBean<GadgetTest>("gadgetTestSharedPro");
        QVERIFY(gadgetTest != nullptr);
        QVERIFY(gadgetTest->text == "gadgetTestSharedPro");
    }
    {
        auto gadgetTest = beanFactory->getBean<GadgetTest>("gadgetTestSharedConstructor");
        QVERIFY(gadgetTest != nullptr);
        QVERIFY(gadgetTest->text == "gadgetTestSharedConstructor");
    }
}

void BeanFactoryTest::objectTestCase()
{
    mcRegisterMetaType<ObjectTest>();
    auto beanFactory = McDefaultBeanFactoryPtr::create();
    {
        auto builder = McObjectClassBeanBuilderPtr::create();
        builder->setClassName("ObjectTest");
        beanFactory->registerBeanBuilder("objectTest", builder);
    }
    {
        auto builder = McObjectClassBeanBuilderPtr::create();
        builder->setClassName("ObjectTest");
        builder->addProperty("text", "objectTestPro");
        beanFactory->registerBeanBuilder("objectTestPro", builder);
    }
    /********************************************************/
    {
        auto objectTest = beanFactory->getBeanPointer<IObjectTest>("objectTest");
        QVERIFY(objectTest != nullptr);
        QVERIFY(objectTest->test() == "objectTest");
    }
}

void BeanFactoryTest::sharedObjectTestCase() {}

void BeanFactoryTest::pluginTestCase() {}

void BeanFactoryTest::sharedPluginTestCase() {}

void BeanFactoryTest::listTestCase() {}

void BeanFactoryTest::mapTestCase() {}
