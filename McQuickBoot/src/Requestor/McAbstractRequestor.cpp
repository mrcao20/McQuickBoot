#include "McBoot/Requestor/McAbstractRequestor.h"

#include <QCoreApplication>
#include <QEvent>
#include <QThreadPool>

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

MC_INIT(McAbstractRequestor)
MC_INIT_END

MC_DECL_PRIVATE_DATA(McAbstractRequestor)
QSharedPointer<QThreadPool> threadPool;
IMcControllerContainerPtr controllerContainer;
McRequestorConfigPtr requestorConfig;
MC_DECL_PRIVATE_DATA_END

McAbstractRequestor::McAbstractRequestor(QObject *parent) : QObject(parent)
{
    MC_NEW_PRIVATE_DATA(McAbstractRequestor);

    d->threadPool = QSharedPointer<QThreadPool>::create();
}

McAbstractRequestor::~McAbstractRequestor()
{
}

void McAbstractRequestor::destroy() noexcept
{
}

qint64 McAbstractRequestor::maxThreadCount() const noexcept
{
    return d->threadPool->maxThreadCount();
}

void McAbstractRequestor::setMaxThreadCount(int val) noexcept
{
    d->threadPool->setMaxThreadCount(val);
}

void McAbstractRequestor::setControllerContainer(IMcControllerContainerConstPtrRef val) noexcept
{
    d->controllerContainer = val;
}

QVariant McAbstractRequestor::syncInvoke(const QString &uri) noexcept
{
    return d->controllerContainer->invoke(uri);
}

QVariant McAbstractRequestor::syncInvoke(const QString &uri, const QJsonObject &data) noexcept
{
    return d->controllerContainer->invoke(uri, data);
}

void McAbstractRequestor::customEvent(QEvent *event)
{
    if (event->type() == QEvent::Type::User + 1) {
        McRunnerEvent *e = static_cast<McRunnerEvent *>(event);
        d->threadPool->start(e->runner());
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

void McAbstractRequestor::finished() noexcept
{
    setMaxThreadCount(d->requestorConfig->maxThreadCount());
}

#include "moc_McAbstractRequestor.cpp"
