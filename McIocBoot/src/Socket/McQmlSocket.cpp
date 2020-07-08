#include "McBoot/Socket/impl/McQmlSocket.h"

#include <QCoreApplication>
#include <QJSValue>
#include <QJSEngine>
#include <QEvent>
#include <QDebug>

class QmlSocketEvent : public QEvent 
{
    Q_GADGET
public:
    enum QmlSocketEventType
    {
        OpenEvent = QEvent::Type::User + 1,
        CloseEvent,
        ErrorEvent,
        MessageEvent
    };
    Q_ENUM(QmlSocketEventType)
    
    QmlSocketEvent(int type, const QVariant &data)
        : QEvent(static_cast<QEvent::Type>(type))
        , m_data(data)
    {}
    ~QmlSocketEvent() override;
    
    QVariant data() const 
    { return m_data; }
    
private:
    MC_PADDING_CLANG(4)
    QVariant m_data;
};

QmlSocketEvent::~QmlSocketEvent()
{
}

MC_DECL_PRIVATE_DATA(McQmlSocket)
MC_PADDING_CLANG(4)
bool isOpenSync{false};
bool isCloseSync{false};
bool isErrorSync{false};
bool isMessageSync{false};
QJSValue onOpen;
QJSValue onClose;
QJSValue onError;
QJSValue onMessage;
MC_DECL_PRIVATE_DATA_END

MC_INIT(McQmlSocket)
qRegisterMetaType<McQmlSocket *>("McQmlSocket");
MC_INIT_END

McQmlSocket::McQmlSocket(QObject *parent)
    : QObject(parent)
{
    MC_NEW_PRIVATE_DATA(McQmlSocket)
}

McQmlSocket::~McQmlSocket() 
{
    close();
}

void McQmlSocket::onOpen(const QJSValue &callback, bool isSync) noexcept 
{
    d->onOpen = callback;
    d->isOpenSync = isSync;
}

void McQmlSocket::onClose(const QJSValue &callback, bool isSync) noexcept 
{
    d->onClose = callback;
    d->isCloseSync = isSync;
}

void McQmlSocket::onError(const QJSValue &callback, bool isSync) noexcept 
{
    d->onError = callback;
    d->isErrorSync = isSync;
}

void McQmlSocket::onMessage(const QJSValue &callback, bool isSync) noexcept 
{
    d->onMessage = callback;
    d->isMessageSync = isSync;
}

void McQmlSocket::opened() noexcept 
{
    if(d->isOpenSync) {
        qApp->postEvent(this, new QmlSocketEvent(QmlSocketEvent::OpenEvent, ""));
    }else{
        open_helper();
    }
}

void McQmlSocket::closed() noexcept 
{
    if(d->isCloseSync) {
        qApp->postEvent(this, new QmlSocketEvent(QmlSocketEvent::CloseEvent, ""));
    }else{
        close_helper();
    }
}

void McQmlSocket::errored(const QString &errMsg) noexcept 
{
    if(d->isErrorSync) {
        qApp->postEvent(this, new QmlSocketEvent(QmlSocketEvent::ErrorEvent, errMsg));
    }else{
        error_helper(errMsg);
    }
}

void McQmlSocket::receivedMsg(const QVariant &msg) noexcept 
{
    if(d->isMessageSync) {
        qApp->postEvent(this, new QmlSocketEvent(QmlSocketEvent::MessageEvent, msg));
    }else{
        message_helper(msg);
    }
    
}

void McQmlSocket::customEvent(QEvent *event) 
{
    int type = event->type();
    switch (type) {
    case QmlSocketEvent::OpenEvent:
        open_helper();
        break;
    case QmlSocketEvent::CloseEvent:
        close_helper();
        break;
    case QmlSocketEvent::ErrorEvent: {
        auto e = static_cast<QmlSocketEvent *>(event);
        error_helper(e->data().toString());
        break;
    }
    case QmlSocketEvent::MessageEvent: {
        auto e = static_cast<QmlSocketEvent *>(event);
        message_helper(e->data());
        break;
    }
    default:
        break;
    }
}

void McQmlSocket::open_helper() noexcept 
{
    if(!d->onOpen.isCallable()) {
        return;
    }
    d->onOpen.call();
}

void McQmlSocket::close_helper() noexcept 
{
    if(!d->onClose.isCallable()) {
        return;
    }
    d->onClose.call();
}

void McQmlSocket::error_helper(const QString &errMsg) noexcept 
{
    if(!d->onError.isCallable()) {
        return;
    }
    d->onError.call(QJSValueList() << QJSValue(errMsg));
}

void McQmlSocket::message_helper(const QVariant &msg) noexcept 
{
    if(!d->onMessage.isCallable()) {
        return;
    }
    auto engine = qjsEngine(this);
    if(!engine) {
        qCritical() << "not found js engine";
        return;
    }
    auto arg = engine->toScriptValue(msg);
    d->onMessage.call(QJSValueList() << arg);
}
