#pragma once

#include "../../McBootGlobal.h"

#include <QEvent>
#include <QVariant>

class McQmlSocketEvent : public QEvent 
{
    Q_GADGET
public:
    enum McQmlSocketEventType 
    {
        SocketInitEvent = QEvent::Type::User + 1,
        QmlSocketErrorEvent,
        QmlSocketSendEvent,
        QmlSocketCloseEvent,
        InnerSocketErrorEvent,
        InnerSocketSendEvent,
        InnerSocketCloseEvent
    };
#if QT_VERSION >= QT_VERSION_CHECK(5, 5, 0)
    Q_ENUM(McQmlSocketEventType)
#else
    Q_ENUMS(McQmlSocketEventType)
#endif
    
    McQmlSocketEvent(int type, const QVariant &data);
    ~McQmlSocketEvent() override;
    
    QVariant data() const {return m_data;}
private:
    QVariant m_data;
};

MC_FORWARD_DECL_PRIVATE_DATA(McQmlSocketRunner)

MC_FORWARD_DECL_CLASS(McSession)
MC_FORWARD_DECL_CLASS(McInnerSocket)

class McQmlSocket;

class MCQUICKBOOT_EXPORT McQmlSocketRunner : public QObject 
{
    Q_OBJECT
public:
    McQmlSocketRunner(QObject *parent = nullptr);
    ~McQmlSocketRunner() override;

    void setSocket(McQmlSocket *socket) noexcept;
    void setSession(McSessionConstPtrRef session) noexcept;
    void setInnerSocket(McInnerSocketConstPtrRef innerSocket) noexcept;
    
    void run() noexcept;
    
protected:
    void customEvent(QEvent *event) override;

private:
    MC_DECL_PRIVATE(McQmlSocketRunner)
};
