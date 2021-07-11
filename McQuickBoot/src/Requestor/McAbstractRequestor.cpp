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
        if (requestorThreadPool->tryStart(e->runner())) {
            return;
        }
        requestorThreadPool->reserveThread();
        connect(e->runner(), &McRequestRunner::signal_finished, this, []() {
            requestorThreadPool->releaseThread();
        });
        requestorThreadPool->start(e->runner());
    }
}

void McAbstractRequestor::run(McAbstractResponse *response,
                              const QString &uri,
                              const QVariant &body) noexcept
{
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
}

#include "moc_McAbstractRequestor.cpp"
