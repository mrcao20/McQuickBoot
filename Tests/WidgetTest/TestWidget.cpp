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
