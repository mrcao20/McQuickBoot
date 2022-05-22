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
#include "TestWidget.h"

#include <QLabel>
#include <QStackedWidget>
#include <QTest>

#include <McIoc/BeanBuilder/Impl/McBeanReference.h>
#include <McIoc/BeanFactory/Impl/McResolverBeanFactory.h>
#include <McWidget/ApplicationContext/Impl/McLocalPathWidgetApplicationContext.h>
#include <McWidget/BeanBuilder/Impl/McWidgetBeanBuilder.h>

void TestWidget::initTestCase()
{
    mcRegisterMetaTypeSimple<QWidget>();
    mcRegisterMetaTypeSimple<QLabel>();
    mcRegisterMetaTypeSimple<CustomWidget>();

    m_appCtx = McLocalPathWidgetApplicationContextPtr::create(":/widgetTest.xml");
    m_appCtx->refresh();
}

void TestWidget::builderCase()
{
    auto beanFactory = McResolverBeanFactoryPtr::create();
    {
        auto builder = McWidgetBeanBuilderPtr::create();
        builder->setClassName("QWidget");
        beanFactory->registerBeanBuilder("widget1", builder);
    }
    {
        auto builder = McWidgetBeanBuilderPtr::create();
        builder->setClassName("QWidget");
        auto ref = McBeanReferencePtr::create();
        ref->setName("widget1");
        builder->setParentBeanReference(ref);
        beanFactory->registerBeanBuilder("widget2", builder);
    }
    {
        auto builder = McWidgetBeanBuilderPtr::create();
        builder->setClassName("CustomWidget");
        QVariantList list;
        {
            auto ref = McBeanReferencePtr::create();
            ref->setName("widget1");
            list.append(QVariant::fromValue(ref));
        }
        {
            auto ref = McBeanReferencePtr::create();
            ref->setName("widget2");
            list.append(QVariant::fromValue(ref));
        }
        builder->addProperty("childs", list);
        beanFactory->registerBeanBuilder("customWidget", builder);
    }
    /********************************************************/
    auto widget1 = beanFactory->getBean<QWidget *>("widget1");
    QVERIFY(widget1 != nullptr);
    QVERIFY(widget1->parentWidget() == nullptr); //!< 单独获取，未指定父对象即为空

    auto widget2 = beanFactory->getBean<QWidget *>("widget2");
    QVERIFY(widget2 != nullptr);
    QVERIFY(widget2->parentWidget() == widget1);

    auto customWidget = beanFactory->getBean<CustomWidget *>("customWidget");
    QVERIFY(customWidget != nullptr);
    QVERIFY(customWidget->parentWidget() == nullptr);
    QVERIFY(customWidget->childs.size() == 2);
    QVERIFY(customWidget->childs.at(0) == widget1);
    QVERIFY(customWidget->childs.at(0)->parentWidget()
            == customWidget); //!< 子对象未指定父对象，则会将当前对象设置为其父对象
    QVERIFY(customWidget->childs.at(1) == widget2);
    QVERIFY(customWidget->childs.at(1)->parentWidget() == widget1); //!< 子对象明确指定了父对象，则其父对象不做更改
}

void TestWidget::appCtxCase()
{
    {
        QVERIFY(m_appCtx->containsBean("widget1"));
        QVERIFY(m_appCtx->isPointer("widget1"));
        auto bean = m_appCtx->getBean<QWidget *>("widget1");
        QVERIFY(bean != nullptr);
        QVERIFY(bean->windowTitle() == QLatin1String("widget1"));
        QVERIFY(bean->parentWidget() == m_appCtx->getBean<QWidget *>("customWidget"));
    }
    {
        QVERIFY(m_appCtx->containsBean("label"));
        QVERIFY(m_appCtx->isPointer("label"));
        auto bean = m_appCtx->getBean<QLabel *>("label");
        QVERIFY(bean != nullptr);
        QVERIFY(bean->windowTitle() == QLatin1String("label"));
        QVERIFY(bean->text() == QLatin1String("label"));
        QVERIFY(bean->parentWidget() == m_appCtx->getBean<QWidget *>("widget1"));
    }
    {
        QVERIFY(m_appCtx->containsBean("customWidget"));
        QVERIFY(m_appCtx->isPointer("customWidget"));
        auto bean = m_appCtx->getBean<CustomWidget *>("customWidget");
        QVERIFY(bean != nullptr);
        QVERIFY(bean->windowTitle() == QLatin1String("customWidget"));
        QVERIFY(bean->childs.size() == 2);
        QVERIFY(bean->childs.at(0) == m_appCtx->getBean<QWidget *>("widget1"));
        QVERIFY(bean->childs.at(1) == m_appCtx->getBean<QWidget *>("label"));
        QVERIFY(bean->parentWidget() == m_appCtx->getBean<QWidget *>("stackWidget"));
    }
    {
        QVERIFY(m_appCtx->containsBean("widget2"));
        QVERIFY(m_appCtx->isPointer("widget2"));
        auto bean = m_appCtx->getBean<QWidget *>("widget2");
        QVERIFY(bean != nullptr);
        QVERIFY(bean->windowTitle() == QLatin1String("widget2"));
        QVERIFY(bean->parentWidget() == m_appCtx->getBean<QWidget *>("stackWidget"));
    }
    {
        QVERIFY(m_appCtx->containsBean("stackWidget"));
        QVERIFY(m_appCtx->isPointer("stackWidget"));
        auto bean = m_appCtx->getBean<QStackedWidget *>("stackWidget");
        QVERIFY(bean != nullptr);
        QVERIFY(bean->windowTitle() == QLatin1String("stackWidget"));
        QVERIFY(bean->lineWidth() == 2);
        QVERIFY(bean->count() == 2);
        QVERIFY(bean->widget(0) == m_appCtx->getBean<QWidget *>("customWidget"));
        QVERIFY(bean->widget(1) == m_appCtx->getBean<QWidget *>("widget2"));
        QVERIFY(bean->parentWidget() == nullptr);
    }
}
