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
#include "XmlApplicationContextTest.h"

#include <QtTest>

#include <McIoc/ApplicationContext/Impl/McLocalPathApplicationContext.h>

#include "BeanFactoryTest.h"
#include "BeanReaderTest.h"

XmlApplicationContextTest::XmlApplicationContextTest(const IMcApplicationContextPtr &appCtx, bool flag)
{
    m_appCtx = appCtx;
    m_appCtx->refresh();
    m_flag = flag;
}

void XmlApplicationContextTest::podCase()
{
    {
        QVERIFY(!m_appCtx->isPointer("podTest"));
        auto bean = m_appCtx->getBean<PodTestPtr>("podTest");
        QVERIFY(!bean.isNull());
        QVERIFY(bean->text == "podTest");
    }
    {
        QVERIFY(m_appCtx->isPointer("podTestPointer"));
        auto bean = m_appCtx->getBean<PodTest *>("podTestPointer");
        QVERIFY(bean != nullptr);
        QVERIFY(bean->text == "podTest");
    }
}

void XmlApplicationContextTest::gadgetCase()
{
    {
        QVERIFY(!m_appCtx->isPointer("gadgetTest"));
        auto bean = m_appCtx->getBean<GadgetTestPtr>("gadgetTest");
        QVERIFY(!bean.isNull());
        QVERIFY(bean->text == "gadgetTest1");
    }
    {
        QVERIFY(m_appCtx->isPointer("gadgetTestPointer"));
        auto bean = m_appCtx->getBean<GadgetTest *>("gadgetTestPointer");
        QVERIFY(bean != nullptr);
        QVERIFY(bean->text == "gadgetTest2");
    }
}

void XmlApplicationContextTest::containerCase()
{
    {
        QVERIFY(!m_appCtx->isPointer("listTest"));
        auto bean = m_appCtx->getBean("listTest").value<QList<QString>>();
        QVERIFY(bean.size() == 3);
        auto debug = qDebug();
        for (auto text : bean) {
            debug << text << " ";
        }
    }
    {
        mcRegisterContainer<QMap<QString, QString>>();
        QVERIFY(!m_appCtx->isPointer("mapTest"));
        auto bean = m_appCtx->getBean("mapTest").value<QMap<QString, QString>>();
        QVERIFY(bean.size() == 2);
        auto debug = qDebug();
        QMapIterator<QString, QString> itr(bean);
        while (itr.hasNext()) {
            auto item = itr.next();
            debug << item.key() << "-" << item.value() << " ";
        }
    }
}

void XmlApplicationContextTest::objectCase()
{
    {
        QVERIFY(!m_appCtx->isPointer("objectTest"));
        QVERIFY(!m_appCtx->isSingleton("objectTest"));
        auto bean = m_appCtx->getBean<ObjectTestPtr>("objectTest");
        QVERIFY(!bean.isNull());
        QVERIFY(bean->test() == "objectTest1");
        auto child = bean->property("object").value<ObjectTestPtr>();
        QVERIFY(!child.isNull());
        QVERIFY(child->test() == "innerObjectTest");
        QVERIFY(bean->parent() != nullptr);
        QVERIFY(bean->parent() == m_appCtx->getBean<QObject *>("objectTestPointer"));
    }
    {
        QVERIFY(m_appCtx->isPointer("objectTestPointer"));
        QVERIFY(m_appCtx->isSingleton("objectTestPointer"));
        auto bean = m_appCtx->getBean<ObjectTest *>("objectTestPointer");
        QVERIFY(bean != nullptr);
        QVERIFY(bean->test() == "objectTestPointer");
        QVERIFY(bean->parent() == nullptr);
        auto child = bean->property("object").value<ObjectTestPtr>();
        QVERIFY(!child.isNull());
        QVERIFY(child->test() == "objectTest1");
        //! objectTest的isSingleton为false，不是单例，每次获取均不一样
        QVERIFY(child != m_appCtx->getBean<QObjectPtr>("objectTest"));
        QVERIFY(child->test() == m_appCtx->getBean<ObjectTestPtr>("objectTest")->test());
        bean->testSignal();
        QVERIFY(bean->property("list").value<QStringList>() == m_appCtx->getBean("listTest").value<QStringList>());
        auto map = bean->property("map").value<QMap<QString, QSharedPointer<ObjectTest>>>();
        QVERIFY(map.size() == 1);
        QVERIFY(map.firstKey() == "jack");
        QVERIFY(!map.first().isNull());
        QVERIFY(map.first()->test() == "objectTest1");
    }
}

void XmlApplicationContextTest::pluginCase()
{
    if (!m_flag) {
        return;
    }
    {
        QVERIFY(!m_appCtx->isContained("pluginTest"));
        QVERIFY(!m_appCtx->isContained("pluginTestPointer"));
    }
#ifdef Q_OS_WIN
    m_appCtx = McLocalPathApplicationContextPtr::create(":/iocTest.xml", "debug");
#else
    m_appCtx = McLocalPathApplicationContextPtr::create(":/iocTest.xml", "linuxDebug");
#endif
    //! 由于插件的唯一性问题，多次加载同一个插件，实例为同一个，如果提前刷新容器，那么插件数据会不准确
    //    m_appCtx->refresh();
    {
        QVERIFY(!m_appCtx->isPointer("pluginTest"));
        auto bean = m_appCtx->getBean<IObjectTestPtr>("pluginTest");
        QVERIFY(!bean.isNull());
        QVERIFY(bean->test() == "pluginTest");
    }
    {
        QVERIFY(m_appCtx->isPointer("pluginTestPointer"));
        auto bean = m_appCtx->getBean<IObjectTest *>("pluginTestPointer");
        QVERIFY(bean != nullptr);
        QVERIFY(bean->test() == "pluginTestPointer");
    }
}
