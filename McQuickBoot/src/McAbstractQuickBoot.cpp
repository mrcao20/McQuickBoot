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
#include "McBoot/McAbstractQuickBoot.h"

#ifdef MC_ENABLE_QSCXML
#include <QScxmlStateMachine>
#endif
#include <QThread>
#include <QWidget>

#include <McIoc/ApplicationContext/IMcApplicationContext.h>
#include <McLog/Configurator/McXMLConfigurator.h>
#include <McWidgetIoc/ApplicationContext/IMcWidgetApplicationContext.h>

#include "McBoot/BeanDefinitionReader/impl/McConfigurationFileBeanDefinitionReader.h"
#include "McBoot/Configuration/McConfigurationContainer.h"
#include "McBoot/Configuration/McLogConfig.h"
#include "McBoot/Configuration/McWidgetConfig.h"
#include "McBoot/Controller/impl/McControllerContainer.h"
#include "McBoot/Model/impl/McModelContainer.h"
#include "McBoot/Service/impl/McServiceContainer.h"

MC_GLOBAL_STATIC_BEGIN(mcAbstractQuickBootStaticData)
McAbstractQuickBootPtr boot;
MC_GLOBAL_STATIC_END(mcAbstractQuickBootStaticData)

MC_DECL_PRIVATE_DATA(McAbstractQuickBoot)
McCppRequestorPtr requestor;
QThread *workThread{nullptr};
MC_DECL_PRIVATE_DATA_END

MC_INIT(McAbstractQuickBoot)
qRegisterMetaType<QThread *>();
MC_DESTROY()
if (!mcAbstractQuickBootStaticData.exists()) {
    return;
}
if (mcAbstractQuickBootStaticData->boot->d->workThread != nullptr) {
    mcAbstractQuickBootStaticData->boot->d->workThread->quit();
    mcAbstractQuickBootStaticData->boot->d->workThread->wait();
    delete mcAbstractQuickBootStaticData->boot->d->workThread;
    mcAbstractQuickBootStaticData->boot->d->workThread = nullptr;
}
mcAbstractQuickBootStaticData->boot.reset();
MC_INIT_END

McAbstractQuickBoot::McAbstractQuickBoot(QObject *parent) : QObject(parent)
{
    MC_NEW_PRIVATE_DATA(McAbstractQuickBoot);
}

McAbstractQuickBoot::~McAbstractQuickBoot() {}

QMetaObject::Connection McAbstractQuickBoot::connect(const QString &sender,
                                                     const QString &signal,
                                                     const QString &receiver,
                                                     const QString &slot,
                                                     Qt::ConnectionType type) noexcept
{
    return Mc::Ioc::connect(McAbstractQuickBoot::instance()->getApplicationContext().data(),
                            sender,
                            signal,
                            receiver,
                            slot,
                            type);
}

QMetaObject::Connection McAbstractQuickBoot::connect(const QString &sender,
                                                     const QString &signal,
                                                     QObject *receiver,
                                                     const QString &slot,
                                                     Qt::ConnectionType type) noexcept
{
    return Mc::Ioc::connect(McAbstractQuickBoot::instance()->getApplicationContext().data(),
                            sender,
                            signal,
                            receiver,
                            slot,
                            type);
}

bool McAbstractQuickBoot::disconnect(const QString &sender,
                                     const QString &signal,
                                     const QString &receiver,
                                     const QString &slot) noexcept
{
    return Mc::Ioc::disconnect(McAbstractQuickBoot::instance()->getApplicationContext().data(),
                               sender,
                               signal,
                               receiver,
                               slot);
}

bool McAbstractQuickBoot::disconnect(const QString &sender,
                                     const QString &signal,
                                     QObject *receiver,
                                     const QString &slot) noexcept
{
    return Mc::Ioc::disconnect(McAbstractQuickBoot::instance()->getApplicationContext().data(),
                               sender,
                               signal,
                               receiver,
                               slot);
}

QSharedPointer<McAbstractQuickBoot> McAbstractQuickBoot::instance() noexcept
{
    McAbstractQuickBootPtr &boot = mcAbstractQuickBootStaticData->boot;
    Q_ASSERT_X(!boot.isNull(), "McAbstractQuickBoot::instance()", "please call init before");
    return boot;
}

void McAbstractQuickBoot::addConfigPath(const QString &path) noexcept
{
    McConfigurationFileBeanDefinitionReader::addConfigPath(path);
}

McCppRequestor &McAbstractQuickBoot::requestor() const noexcept
{
    return *d->requestor.data();
}

void McAbstractQuickBoot::setInstance(const QSharedPointer<McAbstractQuickBoot> &ins) noexcept
{
    mcAbstractQuickBootStaticData->boot = ins;
}

void McAbstractQuickBoot::doRefresh(const QStringList &preloadBeans) noexcept
{
    auto context = getApplicationContext();
    auto reader = McConfigurationFileBeanDefinitionReaderPtr::create(context);
    reader->readBeanDefinition(context.data());
    context->getBean("iocConfig");
    auto configurationContainer = context->getBean<McConfigurationContainer>(
        "configurationContainer");
    configurationContainer->init(this);
    auto workThreadName = QT_STRINGIFY(MC_QUICKBOOT_WORK_THREAD_NAME);
    if (context->containsBean(workThreadName)) {
        d->workThread = context->getBeanPointer<QThread *>(workThreadName);
    }
    if (d->workThread != nullptr && !d->workThread->isRunning()) {
        d->workThread->start();
    }
    for (auto &preloadBean : qAsConst(preloadBeans)) {
        context->getBean(preloadBean, d->workThread);
    }
#ifdef MC_ENABLE_QSCXML
    auto stateMachineName = QT_STRINGIFY(MC_QUICKBOOT_STATE_MACHINE_NAME);
    if (context->containsBean(stateMachineName)) {
        auto val = context->getBeanPointer<QScxmlStateMachine *>(stateMachineName);
        McAbstractRequestor::setStaticStateMachine(val);
    }
#endif
    d->requestor = context->getBean<McCppRequestor>("cppRequestor", d->workThread);
    context->refresh(d->workThread); //!< 预加载bean
    do {
        auto logConfig = context->getBean<McLogConfig>("logConfig");
        if (logConfig.isNull() || logConfig->appCtx().isNull()) {
            break;
        }
        if (logConfig->mergeInto()) {
            logConfig->appCtx()->registerBeanDefinition(context->getBeanDefinitions());
        }
        McXMLConfigurator::configure(logConfig->appCtx(), logConfig->repositoryName());
    } while (false);
    do {
        auto widgetConfig = context->getBean<McWidgetConfig>("widgetConfig");
        if (widgetConfig.isNull() || widgetConfig->appCtx().isNull()) {
            break;
        }
        if (widgetConfig->mergeInto()) {
            widgetConfig->appCtx()->registerBeanDefinition(context->getBeanDefinitions());
        }
        auto w = widgetConfig->appCtx()->getBean(widgetConfig->mainWindowName());
        if (w == nullptr) {
            break;
        }
        Mc::addPostRoutine(widgetConfig->destroyPriority(), [w]() {
            w->hide();
            delete w;
        });
        w->show();
    } while (false);
    auto controllerContainer = context->getBean<McControllerContainer>("controllerContainer");
    controllerContainer->init(this);
    auto modelContainer = context->getBean<McModelContainer>("modelContainer");
    modelContainer->init(this);
    auto serviceContainer = context->getBean<McServiceContainer>("serviceContainer");
    serviceContainer->init(this);
    initContainer();

    d->requestor->setAppCtx(context.data());
}
