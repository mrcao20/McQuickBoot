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
#include "McBoot/Requestor/McAbstractRequestor.h"

#include <QCoreApplication>
#include <QDebug>
#include <QEvent>
#include <QQmlEngine>
#include <QThreadPool>

#include <McIoc/ApplicationContext/IMcApplicationContext.h>

#include "McBoot/Configuration/McRequestorConfig.h"
#include "McBoot/Configuration/McStateMachineConfig.h"
#include "McBoot/Controller/IMcControllerContainer.h"
#include "McBoot/Controller/impl/McAbstractResponse.h"
#include "McBoot/Controller/impl/McRequestRunner.h"
#include "McBoot/Model/IMcModelContainer.h"
#include "McBoot/Utils/Response/IMcResponseHandler.h"
#include "McBoot/Utils/Response/McResponseHandlerFactory.h"

class McRunnerEvent : public QEvent
{
    Q_GADGET
public:
    McRunnerEvent(McRequestRunner *runner)
        : QEvent(static_cast<QEvent::Type>(QEvent::Type::User + 1)), m_runner(runner)
    {}

    ~McRunnerEvent() override;

    McRequestRunner *runner() const { return m_runner; }

private:
    McRequestRunner *m_runner{nullptr};
};

McRunnerEvent::~McRunnerEvent() {}

MC_GLOBAL_STATIC(QThreadPool, requestorThreadPool)
MC_GLOBAL_STATIC(QScxmlStateMachine *, staticStateMachine)

MC_INIT(McAbstractRequestor)
MC_REGISTER_CONTAINER_CONVERTER(QList<IMcResponseHandlerPtr>)
MC_DESTROY(Mc::QuickBoot::ThreadPool)
if (!requestorThreadPool.exists()) {
    return;
}
requestorThreadPool->waitForDone();
MC_INIT_END

MC_DECL_PRIVATE_DATA(McAbstractRequestor)
IMcControllerContainerPtr controllerContainer;
IMcModelContainerPtr modelContainer;
McRequestorConfigPtr requestorConfig;
McStateMachineConfigPtr stateMachineConfig;
IMcApplicationContext *appCtx;
QList<IMcResponseHandlerPtr> responseHanlders;
MC_DECL_PRIVATE_DATA_END

McAbstractRequestor::McAbstractRequestor(QObject *parent) : QObject(parent)
{
    MC_NEW_PRIVATE_DATA(McAbstractRequestor);
}

McAbstractRequestor::~McAbstractRequestor()
{
}

qint64 McAbstractRequestor::maxThreadCount() const noexcept
{
    return requestorThreadPool->maxThreadCount();
}

void McAbstractRequestor::setMaxThreadCount(int val) noexcept
{
    requestorThreadPool->setMaxThreadCount(val);
}

IMcControllerContainerPtr McAbstractRequestor::controllerContainer() const noexcept
{
    return d->controllerContainer;
}

void McAbstractRequestor::setControllerContainer(IMcControllerContainerConstPtrRef val) noexcept
{
    d->controllerContainer = val;
}

IMcApplicationContext *McAbstractRequestor::appCtx() const noexcept
{
    return d->appCtx;
}

void McAbstractRequestor::setAppCtx(IMcApplicationContext *val) noexcept
{
    d->appCtx = val;
}

QObject *McAbstractRequestor::getBean(const QString &name) const noexcept
{
    auto var = getBeanToVariant(name);
    QObject *obj = var.value<QObject *>();
    if (obj == nullptr) {
        obj = var.value<QObjectPtr>().data();
    }
    if (obj == nullptr) {
        qWarning() << "cannot get bean for named:" << name;
    } else {
        QQmlEngine::setObjectOwnership(obj, QQmlEngine::CppOwnership);
    }
    return obj;
}

QObject *McAbstractRequestor::getModel(const QString &name) const noexcept
{
    return d->modelContainer->getModel(name);
}

QScxmlStateMachine *McAbstractRequestor::stateMachine() const noexcept
{
    if (staticStateMachine.exists() && (*staticStateMachine) != nullptr) {
        return *staticStateMachine;
    }
    if (d->stateMachineConfig.isNull()) {
        qFatal("please make sure file 'application.yml' exists");
    }
    auto s = d->stateMachineConfig->stateMachine();
    if (s.isNull()) {
        qFatal("please that the statechart file path is configured correctly");
    }
    return s.data();
}

bool McAbstractRequestor::isLoadStateMachine() const noexcept
{
    if (staticStateMachine.exists() && (*staticStateMachine) != nullptr) {
        return true;
    }
    if (d->stateMachineConfig.isNull()) {
        return false;
    }
    auto s = d->stateMachineConfig->stateMachine();
    if (s.isNull()) {
        return false;
    }
    return true;
}

void McAbstractRequestor::setStaticStateMachine(QScxmlStateMachine *val)
{
    *staticStateMachine = val;
}

void McAbstractRequestor::customEvent(QEvent *event)
{
    if (event->type() == QEvent::Type::User + 1) {
        McRunnerEvent *e = static_cast<McRunnerEvent *>(event);
        if (d->requestorConfig.isNull() || d->requestorConfig->autoIncrease()) {
            if (requestorThreadPool->tryStart(e->runner())) {
                return;
            }
            requestorThreadPool->reserveThread();
            connect(e->runner(), &McRequestRunner::signal_finished, this, []() {
                requestorThreadPool->releaseThread();
            });
        }
        requestorThreadPool->start(e->runner());
    }
}

void McAbstractRequestor::run(McAbstractResponse *response,
                              const QString &uri,
                              const QVariant &body) noexcept
{
    response->setHandlers(d->responseHanlders);
    McRequestRunner *runner = new McRequestRunner();
    runner->setAutoDelete(true);
    runner->setResponse(response);
    runner->setControllerContainer(d->controllerContainer);
    runner->setUri(uri);
    runner->setBody(body);
    qApp->postEvent(this, new McRunnerEvent(runner));
}

QVariant McAbstractRequestor::getBeanToVariant(const QString &name) const noexcept
{
    return d->appCtx->getBeanToVariant(name);
}

void McAbstractRequestor::allFinished() noexcept
{
    int maxThreadCount;
    if (d->requestorConfig.isNull()) {
        maxThreadCount = QThread::idealThreadCount();
    } else {
        maxThreadCount = d->requestorConfig->maxThreadCount();
    }
    setMaxThreadCount(maxThreadCount);
    d->responseHanlders.append(McResponseHandlerFactory::getHandlers());
}

#include "moc_McAbstractRequestor.cpp"
