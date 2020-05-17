#include "McBoot/Socket/impl/McQmlSocketRunner.h"

#include <QCoreApplication>
#include <QThread>
#include <QJSValue>

#include "McBoot/Socket/impl/McInnerSocket.h"
#include "McBoot/Socket/impl/McSession.h"
#include "McBoot/Socket/impl/McQmlSocket.h"

McQmlSocketEvent::McQmlSocketEvent(int type, const QVariant &data)
    : QEvent(static_cast<QEvent::Type>(type))
    , m_data(data)
{}

McQmlSocketEvent::~McQmlSocketEvent() {
}

MC_DECL_PRIVATE_DATA(McQmlSocketRunner)
QThread thread;
McQmlSocket *socket;
McSessionPtr session;
McInnerSocketPtr innerSocket;
MC_DECL_PRIVATE_DATA_END

McQmlSocketRunner::McQmlSocketRunner(QObject *parent)
    : QObject(parent)
{
    MC_NEW_PRIVATE_DATA(McQmlSocketRunner);
    
    moveToThread(&d->thread);
    d->thread.start();
}

McQmlSocketRunner::~McQmlSocketRunner() {
    d->thread.quit();
    //! 由于当前对象本身就是处于另外的线程，所以这里直接使用死循环等待线程结束
    while(!d->thread.isFinished()) {
        QThread::msleep(100);
    }
}

void McQmlSocketRunner::setSocket(McQmlSocket *socket) noexcept {
    d->socket = socket;
}

void McQmlSocketRunner::setSession(McSessionConstPtrRef session) noexcept {
    d->session = session;
}

void McQmlSocketRunner::setInnerSocket(McInnerSocketConstPtrRef innerSocket) noexcept {
    d->innerSocket = innerSocket;
}

//! 全部采用直接连接，然后发送事件给自身，让线程回到自身线程再处理，同时每个请求都需要根据请求先后顺序执行
void McQmlSocketRunner::run() noexcept {
    connect(d->socket, &McQmlSocket::send, this, [this](const QJSValue &msg){
        QVariant var;
        var.setValue(msg);
        qApp->postEvent(this, new McQmlSocketEvent(McQmlSocketEvent::QmlSocketSendEvent, var));
    }, Qt::DirectConnection);
    connect(d->socket, &McQmlSocket::close, this, [this](){
        qApp->postEvent(this, new McQmlSocketEvent(McQmlSocketEvent::QmlSocketCloseEvent, ""));
    }, Qt::DirectConnection);
    
    connect(d->session.data(), &McSession::signal_send, this, [this](const QVariant &msg){
        qApp->postEvent(this, new McQmlSocketEvent(McQmlSocketEvent::InnerSocketSendEvent, msg));
    }, Qt::DirectConnection);
    connect(d->session.data(), &McSession::signal_close, this, [this](){
        qApp->postEvent(this, new McQmlSocketEvent(McQmlSocketEvent::InnerSocketCloseEvent, ""));
    }, Qt::DirectConnection);
    
    if(d->innerSocket) {
        qApp->postEvent(this, new McQmlSocketEvent(McQmlSocketEvent::SocketInitEvent, ""));
    }
}

void McQmlSocketRunner::customEvent(QEvent *event) {
    int type = event->type();
    switch (type) {
    case McQmlSocketEvent::SocketInitEvent:
        d->socket->opened();
        d->innerSocket->opened(d->session);
        break;
    case McQmlSocketEvent::QmlSocketErrorEvent: {
        auto e = static_cast<McQmlSocketEvent *>(event);
        d->innerSocket->errored(d->session, e->data().toString());
        this->deleteLater();
        break;
    }
    case McQmlSocketEvent::QmlSocketSendEvent: {
        auto e = static_cast<McQmlSocketEvent *>(event);
        QJSValue msg = e->data().value<QJSValue>();
        QVariant var;
        if(msg.isQObject()) {
            var.setValue(msg.toQObject());
        }else{
            var = msg.toVariant();
        }
        d->innerSocket->receivedMsg(d->session, var);
        break;
    }
    case McQmlSocketEvent::QmlSocketCloseEvent:
        d->innerSocket->closed(d->session);
        this->deleteLater();
        break;
    case McQmlSocketEvent::InnerSocketErrorEvent: {
        auto e = static_cast<McQmlSocketEvent *>(event);
        d->socket->errored(e->data().toString());
        this->deleteLater();
        break;
    }
    case McQmlSocketEvent::InnerSocketSendEvent: {
        auto e = static_cast<McQmlSocketEvent *>(event);
        d->socket->receivedMsg(e->data());
        break;
    }
    case McQmlSocketEvent::InnerSocketCloseEvent:
        d->socket->closed();
        this->deleteLater();
        break;
    default:
        break;
    }
}
