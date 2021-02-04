#include "McBoot/Requestor/McAbstractRequestor.h"

#include <QCoreApplication>
#include <QDebug>
#include <QEvent>
#include <QQmlEngine>
#include <QThreadPool>

#include <McIoc/ApplicationContext/IMcApplicationContext.h>

#include "McBoot/Configuration/McRequestorConfig.h"
#include "McBoot/Controller/IMcControllerContainer.h"
#include "McBoot/Controller/impl/McAbstractResponse.h"
#include "McBoot/Controller/impl/McRequestRunner.h"

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

MC_INIT(McAbstractRequestor)
MC_DESTROY()
if (!requestorThreadPool.exists()) {
    return;
}
requestorThreadPool->waitForDone();
MC_INIT_END

MC_DECL_PRIVATE_DATA(McAbstractRequestor)
IMcControllerContainerPtr controllerContainer;
McRequestorConfigPtr requestorConfig;
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

void McAbstractRequestor::allFinished() noexcept
{
    setMaxThreadCount(d->requestorConfig->maxThreadCount());
}

QVariant McAbstractRequestor::getBeanToVariant(const QString &name) const noexcept
{
    return d->appCtx->getBeanToVariant(name);
}

#include "moc_McAbstractRequestor.cpp"
