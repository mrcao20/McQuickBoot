#include "McBoot/Controller/impl/McRequestRunner.h"

#include <QVariant>
#include <QPointer>
#include <QDebug>

#include "McBoot/Controller/IMcControllerContainer.h"
#include "McBoot/Controller/impl/McAbstractResponse.h"

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
    auto body = d->controllerContainer->invoke(d->uri, d->body);
    if(d->response.isNull()) {  //!< Response可能被QML析构
        qCritical() << "response is null. it's maybe destroyed of qmlengine";
        return;
    }
    d->response->setBody(body);

    emit signal_finished();
}
