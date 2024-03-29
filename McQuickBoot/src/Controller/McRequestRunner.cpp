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
        d->response->setStarted();
        req.setCancel(d->response->getCancel());
        req.setPause(d->response->getPause());
        req.setProgress(d->response->getProgress());
    }
    auto body = d->controllerContainer->invoke(d->uri, d->body, req);
    if(d->response.isNull()) {  //!< Response可能被QML析构
        qCritical() << "response is null. it's maybe destroyed of qmlengine";
        return;
    }
    d->response->setBody(body);
}
