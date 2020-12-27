#include "McBoot/Socket/impl/McQmlSocketContainer.h"

#include <QCoreApplication>
#include <QJSValue>
#include <QMap>
#include <QMetaMethod>
#include <QDebug>

#include <McIoc/ApplicationContext/IMcApplicationContext.h>

#include "McBoot/IMcQuickBoot.h"
#include "McBoot/Socket/impl/McQmlSocket.h"
#include "McBoot/Socket/impl/McInnerSocket.h"
#include "McBoot/Socket/impl/McSession.h"
#include "McBoot/Socket/impl/McQmlSocketRunner.h"

MC_INIT(McQmlSocketContainer)
MC_REGISTER_BEAN_FACTORY(McQmlSocketContainer)
MC_INIT_END

MC_DECL_PRIVATE_DATA(McQmlSocketContainer)
QMap<QString, QObjectPtr> sockets;
MC_DECL_PRIVATE_DATA_END

McQmlSocketContainer::McQmlSocketContainer(QObject *parent)
    : QObject(parent)
{
    MC_NEW_PRIVATE_DATA(McQmlSocketContainer)
}

McQmlSocketContainer::~McQmlSocketContainer() 
{
}

void McQmlSocketContainer::init(const IMcQuickBoot *boot) noexcept
{
    d->sockets.clear();
    auto appCtx = boot->getApplicationContext();
    auto beanNames = Mc::getComponents(appCtx, MC_QML_SOCKET_TAG);
    for (const auto &beanName : beanNames) {
        auto obj = appCtx->getBean(beanName);
        if(!obj) {
            qCritical() << QString("socket for named '%1' not exists").arg(beanName);
            continue;
        }
        d->sockets.insert(beanName, obj);
    }
}

McQmlSocket *McQmlSocketContainer::addConnect(const QString &uri) noexcept 
{
    auto qs = new McQmlSocket();
    
    addConnect(qs, uri);
    
    return qs;
}

McQmlSocket *McQmlSocketContainer::addConnect(const QString &uri, const QJSValue &data) noexcept 
{
    auto qs = new McQmlSocket();
    bool isSync = data.hasProperty("isOpenSync") ? data.property("isOpenSync").toBool() : false;
    if(data.hasProperty("onOpen"))
        qs->onOpen(data.property("onOpen"), isSync);
    isSync = data.hasProperty("isCloseSync") ? data.property("isCloseSync").toBool() : false;
    if(data.hasProperty("onClose"))
        qs->onClose(data.property("onClose"), isSync);
    isSync = data.hasProperty("isErrorSync") ? data.property("isErrorSync").toBool() : false;
    if(data.hasProperty("onError"))
        qs->onError(data.property("onError"), isSync);
    isSync = data.hasProperty("isMessageSync") ? data.property("isMessageSync").toBool() : false;
    if(data.hasProperty("onMessage"))
        qs->onMessage(data.property("onMessage"), isSync);
    
    addConnect(qs, uri);
    
    return qs;
}

void McQmlSocketContainer::addConnect(McQmlSocket *socket, const QString &uri) noexcept 
{
    McSessionPtr session = McSessionPtr::create();
    McQmlSocketRunner *runner = new McQmlSocketRunner();
    
    McInnerSocketPtr innerSocket;
    
    if(d->sockets.contains(uri)) {
        innerSocket = getMethods(d->sockets[uri]);
    }
    
    runner->setSocket(socket);
    runner->setSession(session);
    runner->setInnerSocket(innerSocket);
    runner->run();
    
    if(!innerSocket) {
        qApp->postEvent(runner, new McQmlSocketEvent(
                        McQmlSocketEvent::InnerSocketErrorEvent
                        , "not exists the socket for named: " + uri));
    }
}

McInnerSocketPtr McQmlSocketContainer::getMethods(QObjectConstPtrRef bean) noexcept 
{
    if(!bean) {
        return McInnerSocketPtr();
    }
    
    McInnerSocketPtr innerSocket = McInnerSocketPtr::create();
    
    auto mo = bean->metaObject();
    for(int i = 0; i < mo->methodCount(); ++i) {
        auto method = mo->method(i);
        QString tags = method.tag();
        if(tags.contains(MC_MACRO_STR(MC_ON_OPEN))) {
            innerSocket->setOnOpen(method);
        }else if(tags.contains(MC_MACRO_STR(MC_ON_CLOSE))) {
            innerSocket->setOnClose(method);
        }else if(tags.contains(MC_MACRO_STR(MC_ON_ERROR))) {
            innerSocket->setOnError(method);
        }else if(tags.contains(MC_MACRO_STR(MC_ON_MESSAGE))) {
            innerSocket->setOnMessage(method);
        }
    }
    
    innerSocket->setInnerSocketObj(bean);
    
    return innerSocket;
}
