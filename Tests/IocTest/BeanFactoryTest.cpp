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

#include "McIoc/BeanBuilder/Impl/McSimpleBeanBuilder.h"
#include <McCore/Utils/Impl/McNormalPluginChecker.h>
#include <McIoc/BeanBuilder/Impl/McBeanReference.h>
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
    {
        auto builder = McObjectClassBeanBuilderPtr::create();
        builder->setReferenceResolver(beanFactory.data());
        builder->setClassName("ObjectTest");
        builder->addConstructorArg(0, "objectTestConstructor");
        beanFactory->registerBeanBuilder("objectTestConstructor", builder);
    }
    /********************************************************/
    {
        auto objectTest = beanFactory->getBeanPointer<IObjectTest>("objectTest");
        QVERIFY(objectTest != nullptr);
        QVERIFY(objectTest->test() == "objectTest");
    }
    {
        auto objectTest = beanFactory->getBeanPointer<IObjectTest>("objectTestPro");
        QVERIFY(objectTest != nullptr);
        QVERIFY(objectTest->test() == "objectTestPro");
    }
    {
        auto objectTest = beanFactory->getBeanPointer<IObjectTest>("objectTestConstructor");
        QVERIFY(objectTest != nullptr);
        QVERIFY(objectTest->test() == "objectTestConstructor");
    }
}

void BeanFactoryTest::sharedObjectTestCase()
{
    auto beanFactory = McDefaultBeanFactoryPtr::create();
    {
        auto builder = McSharedObjectClassBeanBuilderPtr::create();
        builder->setReferenceResolver(beanFactory.data());
        builder->setClassName("ObjectTest");
        beanFactory->registerBeanBuilder("objectTestShared", builder);
    }
    {
        auto builder = McSharedObjectClassBeanBuilderPtr::create();
        builder->setReferenceResolver(beanFactory.data());
        builder->setClassName("ObjectTest");
        builder->addProperty("text", "objectTestSharedPro");
        beanFactory->registerBeanBuilder("objectTestSharedPro", builder);
    }
    {
        auto builder = McSharedObjectClassBeanBuilderPtr::create();
        builder->setReferenceResolver(beanFactory.data());
        builder->setClassName("ObjectTest");
        builder->addConstructorArg("val", "objectTestSharedConstructor");
        beanFactory->registerBeanBuilder("objectTestSharedConstructor", builder);
    }
    /********************************************************/
    {
        auto objectTest = beanFactory->getBean<IObjectTest>("objectTestShared");
        QVERIFY(objectTest != nullptr);
        QVERIFY(objectTest->test() == "objectTest");
    }
    {
        auto objectTest = beanFactory->getBean<IObjectTest>("objectTestSharedPro");
        QVERIFY(objectTest != nullptr);
        QVERIFY(objectTest->test() == "objectTestSharedPro");
    }
    {
        auto objectTest = beanFactory->getBean<IObjectTest>("objectTestSharedConstructor");
        QVERIFY(objectTest != nullptr);
        QVERIFY(objectTest->test() == "objectTestSharedConstructor");
    }
}

void BeanFactoryTest::pluginTestCase()
{
    auto beanFactory = McDefaultBeanFactoryPtr::create();
#ifdef QT_DEBUG
    QString filePath("./SimplePlugind.dll");
#else
    QString filePath("./SimplePlugin.dll");
#endif
#ifdef Q_OS_LINUX
    filePath.prepend("../lib/");
    filePath.replace(".dll", ".so");
#endif
    {
        QJsonObject json;
        json.insert("IID", IObjectTest_IID);
#ifdef QT_DEBUG
        json.insert("debug", true);
#endif
        json.insert("className", "SimplePlugin");
        QJsonObject metaJson;
        metaJson.insert("checkKey", "simplePlugin");
        json.insert("MetaData", metaJson);
        auto checker = McNormalPluginCheckerPtr::create(json);
        auto checkerBuilder = McSimpleBeanBuilderPtr::create(QVariant::fromValue(checker));
        beanFactory->registerBeanBuilder("checkerTrue", checkerBuilder);
        auto builder = McObjectPluginBeanBuilderPtr::create();
        builder->setReferenceResolver(beanFactory.data());
        builder->setPluginPath(filePath);
        auto ref = McBeanReferencePtr::create();
        ref->setName("checkerTrue");
        builder->setPluginChecker(ref);
        beanFactory->registerBeanBuilder("pluginTest", builder);
    }
    {
        QJsonObject json;
        json.insert("className", "SimplePlugin1");
        auto checker = McNormalPluginCheckerPtr::create(json);
        auto checkerBuilder = McSimpleBeanBuilderPtr::create(QVariant::fromValue(checker));
        beanFactory->registerBeanBuilder("checkerFalse", checkerBuilder);
        auto builder = McObjectPluginBeanBuilderPtr::create();
        builder->setReferenceResolver(beanFactory.data());
        builder->setPluginPath(filePath);
        auto ref = McBeanReferencePtr::create();
        ref->setName("checkerFalse");
        builder->setPluginChecker(ref);
        beanFactory->registerBeanBuilder("pluginTestFalse", builder);
    }
    {
        auto builder = McObjectPluginBeanBuilderPtr::create();
        builder->setPluginPath(filePath);
        builder->addProperty("text", "pluginTestPro");
        beanFactory->registerBeanBuilder("pluginTestPro", builder);
    }
    /********************************************************/
    {
        auto pluginTest = beanFactory->getBeanPointer<IObjectTest>("pluginTest");
        QVERIFY(pluginTest != nullptr);
        QVERIFY(pluginTest->test() == "SimplePlugin");
    }
    {
        auto pluginTest = beanFactory->getBeanPointer<IObjectTest>("pluginTestFalse");
        QVERIFY(pluginTest == nullptr);
    }
    {
        auto pluginTest = beanFactory->getBeanPointer<IObjectTest>("pluginTestPro");
        QVERIFY(pluginTest != nullptr);
        QVERIFY(pluginTest->test() == "pluginTestPro");
    }
}

void BeanFactoryTest::sharedPluginTestCase()
{
    //! 由于插件的特殊性，pluginTestCase和本用例使用的是同一个插件实例。
    //! 所以Shared类型只能使用一次，否则会造成多次析构的错误。
    auto beanFactory = McDefaultBeanFactoryPtr::create();
#ifdef QT_DEBUG
    QString filePath("./SimplePlugind.dll");
#else
    QString filePath("./SimplePlugin.dll");
#endif
#ifdef Q_OS_LINUX
    filePath.prepend("../lib/");
    filePath.replace(".dll", ".so");
#endif
    {
        auto builder = McSharedObjectPluginBeanBuilderPtr::create();
        builder->setPluginPath(filePath);
        builder->addProperty("text", "pluginTestPro");
        beanFactory->registerBeanBuilder("pluginTestPro", builder);
    }
    /********************************************************/
    {
        auto pluginTest = beanFactory->getBean<IObjectTest>("pluginTestPro");
        QVERIFY(pluginTest != nullptr);
        QVERIFY(pluginTest->test() == "pluginTestPro");
    }
}

void BeanFactoryTest::listTestCase()
{
    auto beanFactory = McDefaultBeanFactoryPtr::create();
    {
        auto builder = McSharedGadgetBeanBuilderPtr::create();
        builder->setReferenceResolver(beanFactory.data());
        builder->setClassName("PodTest");
        beanFactory->registerBeanBuilder("podTestShared", builder);
    }
    {
        auto builder = McSharedObjectClassBeanBuilderPtr::create();
        builder->setReferenceResolver(beanFactory.data());
        builder->setClassName("ObjectTest");
        beanFactory->registerBeanBuilder("objectTestShared", builder);
    }
    /********************************************************/
    {
        auto builder = McListBeanBuilderPtr::create();
        builder->setReferenceResolver(beanFactory.data());
        auto ref = McBeanReferencePtr::create();
        ref->setName("podTestShared");
        builder->addValue(QVariant::fromValue(ref));
        beanFactory->registerBeanBuilder("podList", builder);
    }
    {
        auto builder = McListBeanBuilderPtr::create();
        builder->setReferenceResolver(beanFactory.data());
        auto ref = McBeanReferencePtr::create();
        ref->setName("objectTestShared");
        builder->addValue(QVariant::fromValue(ref));
        beanFactory->registerBeanBuilder("objectList", builder);
    }
    /********************************************************/
    {
        mcRegisterContainerConverter<QList<PodTestPtr>>();
        auto podList = beanFactory->getBeanToVariant("podList").value<QList<PodTestPtr>>();
        QVERIFY(podList.size() == 1);
        auto debug = qDebug();
        for (auto pod : podList) {
            debug << pod->text << " ";
        }
    }
    {
        mcRegisterContainerConverter<QList<IObjectTestPtr>>();
        auto podList = beanFactory->getBeanToVariant("objectList").value<QList<IObjectTestPtr>>();
        QVERIFY(podList.size() == 1);
        auto debug = qDebug();
        for (auto pod : podList) {
            debug << pod->test() << " ";
        }
    }
}

void BeanFactoryTest::mapTestCase()
{
    auto beanFactory = McDefaultBeanFactoryPtr::create();
    {
        auto builder = McSharedGadgetBeanBuilderPtr::create();
        builder->setReferenceResolver(beanFactory.data());
        builder->setClassName("PodTest");
        beanFactory->registerBeanBuilder("podTestShared", builder);
    }
    {
        auto builder = McSharedObjectClassBeanBuilderPtr::create();
        builder->setReferenceResolver(beanFactory.data());
        builder->setClassName("ObjectTest");
        beanFactory->registerBeanBuilder("objectTestShared", builder);
    }
    /********************************************************/
    {
        auto builder = McMapBeanBuilderPtr::create();
        builder->setReferenceResolver(beanFactory.data());
        {
            auto ref = McBeanReferencePtr::create();
            ref->setName("podTestShared");
            builder->addValue("pod1", QVariant::fromValue(ref));
        }
        {
            auto ref = McBeanReferencePtr::create();
            ref->setName("podTestShared");
            builder->addValue("pod2", QVariant::fromValue(ref));
        }
        beanFactory->registerBeanBuilder("podMap", builder);
    }
    {
        auto builder = McMapBeanBuilderPtr::create();
        builder->setReferenceResolver(beanFactory.data());
        {
            auto ref = McBeanReferencePtr::create();
            ref->setName("objectTestShared");
            builder->addValue("obj1", QVariant::fromValue(ref));
        }
        {
            auto ref = McBeanReferencePtr::create();
            ref->setName("objectTestShared");
            builder->addValue("obj2", QVariant::fromValue(ref));
        }
        beanFactory->registerBeanBuilder("objMap", builder);
    }
    /********************************************************/
    {
        mcRegisterContainerConverter<QMap<QString, PodTestPtr>>();
        auto podMap = beanFactory->getBeanToVariant("podMap").value<QMap<QString, PodTestPtr>>();
        QVERIFY(podMap.size() == 2);
        auto debug = qDebug();
        QMapIterator<QString, PodTestPtr> itr(podMap);
        while (itr.hasNext()) {
            itr.next();
            debug << itr.key() << " " << itr.value().data() << "; ";
        }
    }
    {
        mcRegisterContainerConverter<QMap<QString, IObjectTestPtr>>();
        auto objMap = beanFactory->getBeanToVariant("objMap").value<QMap<QString, IObjectTestPtr>>();
        QVERIFY(objMap.size() == 2);
        auto debug = qDebug();
        QMapIterator<QString, IObjectTestPtr> itr(objMap);
        while (itr.hasNext()) {
            itr.next();
            debug << itr.key() << " " << itr.value().data() << "; ";
        }
    }
}
