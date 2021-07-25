#include "McBoot/Controller/impl/McRequestRunner.h"

#include <QDebug>
#include <QPointer>
#include <QScopeGuard>
#include <QVariant>

#include "McBoot/Controller/IMcControllerContainer.h"
#include "McBoot/Controller/impl/McAbstractResponse.h"
#include "McBoot/Requestor/McRequest.h"

MC_DECL_PRIVATE_DATA(McRequestRunner)
QPointer<McAbstractResponse> response;
IMcControllerContainerPtr controllerContainer;
QString uri;
QVariant body;
MC_DECL_PRIVATE_DATA_END

McRequestRunner::McRequestRunner()
{
    MC_NEW_PRIVATE_DATA(McRequestRunner)
}

McRequestRunner::~McRequestRunner() 
{
}

void McRequestRunner::setResponse(McAbstractResponse *val) noexcept
{
    d->response = val;
}

void McRequestRunner::setControllerContainer(IMcControllerContainerConstPtrRef val) noexcept 
{
    d->controllerContainer = val;
}

void McRequestRunner::setUri(const QString &uri) noexcept 
{
    d->uri = uri;
}

void McRequestRunner::setBody(const QVariant &body) noexcept
{
    d->body = body;
}

void McRequestRunner::run() 
{
    auto cleanup = qScopeGuard([this]() { emit signal_finished(); });
    McRequest req;
    if (!d->response.isNull()) {
        req.setCancel(d->response->getCancel());
        req.setProgress(d->response->getProgress());
    }
    auto body = d->controllerContainer->invoke(d->uri, d->body, req);
    if(d->response.isNull()) {  //!< Response可能被QML析构
        qCritical() << "response is null. it's maybe destroyed of qmlengine";
        return;
    }
    d->response->setBody(body);
}
