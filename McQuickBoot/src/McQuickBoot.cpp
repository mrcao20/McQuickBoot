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
#include "McBoot/McQuickBoot.h"

#include <QDebug>
#include <QGlobalStatic>
#include <QJSValue>
#include <QQmlContext>
#include <QQuickView>
#include <QQuickWidget>

#include <McIoc/ApplicationContext/impl/McAnnotationApplicationContext.h>
#include <McIoc/BeanDefinition/IMcBeanDefinition.h>

#include "McBoot/Requestor/McQmlRequestor.h"

namespace {

struct McQuickBootStaticData
{
    McQuickBootPtr boot;
    function<void(QCoreApplication *)> preInitFunc;
    function<void(QCoreApplication *, QQmlApplicationEngine *)> afterInitFunc;
};

}

MC_GLOBAL_STATIC(McQuickBootStaticData, mcQuickBootStaticData)

MC_DECL_PRIVATE_DATA(McQuickBoot)
McAnnotationApplicationContextPtr context;
QQmlEngine *engine{nullptr};
MC_DECL_PRIVATE_DATA_END

MC_INIT(McQuickBoot)
MC_DESTROY()
if (!mcQuickBootStaticData.exists()) {
    return;
}
mcQuickBootStaticData->boot.reset();
MC_INIT_END

McQuickBoot::McQuickBoot(QObject *parent) : McAbstractQuickBoot(parent)
{
    MC_NEW_PRIVATE_DATA(McQuickBoot);
}

McQuickBoot::~McQuickBoot()
{
}

void McQuickBoot::init(QCoreApplication *app, QQmlApplicationEngine *engine) noexcept
{
    if(mcQuickBootStaticData->preInitFunc) {
        mcQuickBootStaticData->preInitFunc(app);
    }
    
    if(!mcQuickBootStaticData->boot.isNull()) {
        qWarning("the boot is already init.");
        return;
    }
    mcQuickBootStaticData->boot = McQuickBootPtr::create();
    McAbstractQuickBoot::setInstance(mcQuickBootStaticData->boot);
    McQuickBootPtr &boot = mcQuickBootStaticData->boot;
    boot->initBoot(engine);
    auto appCtx = boot->d->context;

    auto requestor = appCtx->getBeanPointer<McQmlRequestor>("qmlRequestor");
    requestor->setAppCtx(appCtx.data());

    //! engine的newQObject函数会将其参数所有权转移到其返回的QJSValue中
    QJSValue jsObj = engine->newQObject(requestor);
    engine->globalObject().setProperty("$", jsObj);
    engine->importModule(":/requestor.mjs");

    if(mcQuickBootStaticData->afterInitFunc) {
        mcQuickBootStaticData->afterInitFunc(app, engine);
    }
}

QQmlEngine *McQuickBoot::engine() noexcept
{
    return mcQuickBootStaticData->boot->d->engine;
}

QQuickView *McQuickBoot::createQuickView(const QString &source, QWindow *parent) noexcept
{
    if(McQuickBoot::engine() == nullptr) {
        qCritical("engine is null. you must be call function init or run before\n");
        return nullptr;
    }
    QQuickView *view = new QQuickView(engine(), parent);
    view->setSource(QUrl(Mc::toAbsolutePath(source)));
    return view;
}

QQuickWidget *McQuickBoot::createQuickWidget(const QString &source, QWidget *parent) noexcept
{
    if (McQuickBoot::engine() == nullptr) {
        qCritical("engine is null. you must be call function init or run before\n");
        return nullptr;
    }
    QQuickWidget *widget = new QQuickWidget(engine(), parent);
    widget->setSource(QUrl(Mc::toAbsolutePath(source)));
    return widget;
}

QSharedPointer<McQuickBoot> McQuickBoot::instance() noexcept
{
    McQuickBootPtr &boot = mcQuickBootStaticData->boot;
    Q_ASSERT_X(!boot.isNull(), "McQuickBoot::instance()", "please call init before");
    return boot;
}

void McQuickBoot::setPreInitFunc(const function<void(QCoreApplication *)> &func) noexcept
{
    mcQuickBootStaticData->preInitFunc = func;
}

void McQuickBoot::setAfterInitFunc(const function<void(QCoreApplication *, QQmlApplicationEngine *)> &func) noexcept
{
    mcQuickBootStaticData->afterInitFunc = func;
}

void McQuickBoot::initBoot(QQmlEngine *engine) noexcept
{
    if (d->context) {
        qInfo() << "The container has been initialized";
        return;
    }
    d->engine = engine;
    d->context = McAnnotationApplicationContextPtr::create();
    doRefresh(QStringList() << "qmlRequestor");
}

void McQuickBoot::refresh() noexcept
{
    d->context->generateReader();
    doRefresh();
}

IMcApplicationContextPtr McQuickBoot::getApplicationContext() const noexcept 
{
    return d->context;
}

void McQuickBoot::initContainer() const noexcept
{
}
