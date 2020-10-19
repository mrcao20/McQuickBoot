#include "McBoot/Requestor/McQmlRequestor.h"

#include <QCoreApplication>
#include <QThreadPool>
#include <QQmlEngine>
#include <QEvent>
#include <QDebug>

#include "McBoot/Controller/IMcControllerContainer.h"
#include "McBoot/Controller/impl/McQmlResponse.h"
#include "McBoot/Controller/impl/McRequestRunner.h"
#include "McBoot/Socket/IMcQmlSocketContainer.h"
#include "McBoot/Socket/impl/McQmlSocket.h"
#include "McBoot/Configuration/McQmlRequestorConfig.h"

class McRunnerEvent : public QEvent 
{
    Q_GADGET
public:
    McRunnerEvent(McRequestRunner *runner)
        : QEvent(static_cast<QEvent::Type>(QEvent::Type::User + 1))
        , m_runner(runner)
    {}
    
    ~McRunnerEvent() override;
    
    McRequestRunner *runner() const 
    { return m_runner; }
    
private:
    MC_PADDING_CLANG(4)
    McRequestRunner *m_runner{nullptr};
};

McRunnerEvent::~McRunnerEvent() 
{
}

MC_INIT(McQmlRequestor)
MC_REGISTER_BEAN_FACTORY(MC_TYPELIST(McQmlRequestor))
MC_INIT_END

MC_DECL_PRIVATE_DATA(McQmlRequestor)
QSharedPointer<QThreadPool> threadPool;
IMcControllerContainerPtr controllerContainer;
IMcQmlSocketContainerPtr qmlSocketContainer;
McQmlRequestorConfigPtr requestorConfig;
MC_DECL_PRIVATE_DATA_END

McQmlRequestor::McQmlRequestor(QObject *parent)
    : QObject(parent)
{
    MC_NEW_PRIVATE_DATA(McQmlRequestor);
    
    d->threadPool = QSharedPointer<QThreadPool>::create();
}

McQmlRequestor::~McQmlRequestor() 
{
    qDebug() << "~McQmlRequestor";
}

void McQmlRequestor::destroy() noexcept
{
    qDebug() << "McQmlRequestor is called destroy";
}

qint64 McQmlRequestor::maxThreadCount() const noexcept 
{
    return d->threadPool->maxThreadCount();
}

void McQmlRequestor::setMaxThreadCount(int val) noexcept 
{
    d->threadPool->setMaxThreadCount(val);
}

void McQmlRequestor::setControllerContainer(IMcControllerContainerConstPtrRef val) noexcept 
{
    d->controllerContainer = val;
}

void McQmlRequestor::setSocketContainer(IMcQmlSocketContainerConstPtrRef val) noexcept 
{
    d->qmlSocketContainer = val;
}

McQmlResponse *McQmlRequestor::invoke(const QString &uri) noexcept 
{
    auto response = new McQmlResponse();
    QQmlEngine::setObjectOwnership(response, QQmlEngine::CppOwnership);
    run(response, uri, QVariant());
    return response;    //!< 没有指定父对象，该对象将在整个请求完毕时被析构
}

McQmlResponse *McQmlRequestor::invoke(const QString &uri, const QJsonObject &data) noexcept 
{
    auto response = new McQmlResponse();
    QQmlEngine::setObjectOwnership(response, QQmlEngine::CppOwnership);
    run(response, uri, data);
    return response;    //!< 没有指定父对象，该对象将在整个请求完毕时被析构
}

QVariant McQmlRequestor::syncInvoke(const QString &uri) noexcept
{
    return d->controllerContainer->invoke(uri);
}

QVariant McQmlRequestor::syncInvoke(const QString &uri, const QJsonObject &data) noexcept
{
    return d->controllerContainer->invoke(uri, data);
}

McQmlSocket *McQmlRequestor::addConnect(const QString &uri) noexcept 
{
    auto qs = d->qmlSocketContainer->addConnect(uri);
    QQmlEngine::setObjectOwnership(qs, QQmlEngine::JavaScriptOwnership);
    return qs;
}

McQmlSocket *McQmlRequestor::addConnect(const QString &uri, const QJSValue &data) noexcept 
{
    auto qs = d->qmlSocketContainer->addConnect(uri, data);
    QQmlEngine::setObjectOwnership(qs, QQmlEngine::JavaScriptOwnership);
    return qs;
}

void McQmlRequestor::customEvent(QEvent *event) 
{
    if(event->type() == QEvent::Type::User + 1) {
        McRunnerEvent *e = static_cast<McRunnerEvent *>(event);
        d->threadPool->start(e->runner());
    }
}

void McQmlRequestor::finished() noexcept
{
    setMaxThreadCount(d->requestorConfig->maxThreadCount());
}

void McQmlRequestor::run(McQmlResponse *response, const QString &uri, const QVariant &body) noexcept 
{
    McRequestRunner *runner = new McRequestRunner();
    runner->setAutoDelete(true);
    runner->setResponse(response);
    runner->setControllerContainer(d->controllerContainer);
    runner->setUri(uri);
    runner->setBody(body);
    qApp->postEvent(this, new McRunnerEvent(runner));
}

#include "moc_McQmlRequestor.cpp"
