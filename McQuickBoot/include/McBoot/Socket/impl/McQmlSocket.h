#pragma once

#include "../../McBootGlobal.h"

QT_BEGIN_NAMESPACE
class QJSValue;
QT_END_NAMESPACE

MC_FORWARD_DECL_PRIVATE_DATA(McQmlSocket);

class MCQUICKBOOT_EXPORT McQmlSocket : public QObject 
{
    Q_OBJECT
    MC_DECL_INIT(McQmlSocket)
public:
    explicit McQmlSocket(QObject *parent = nullptr);
    ~McQmlSocket() override;

    Q_INVOKABLE void onOpen(const QJSValue &callback, bool isSync = false) noexcept;
    Q_INVOKABLE void onClose(const QJSValue &callback, bool isSync = false) noexcept;
    Q_INVOKABLE void onError(const QJSValue &callback, bool isSync = false) noexcept;
    Q_INVOKABLE void onMessage(const QJSValue &callback, bool isSync = false) noexcept;

    void opened() noexcept;
    void closed() noexcept;
    void errored(const QString &errMsg) noexcept;
    void receivedMsg(const QVariant &msg) noexcept;

signals:
    void send(const QJSValue &);
    void close();
    
protected:
    void customEvent(QEvent *event) override;
    
private:
    void open_helper() noexcept;
    void close_helper() noexcept;
    void error_helper(const QString &errMsg) noexcept;
    void message_helper(const QVariant &msg) noexcept;
    
private:
    MC_DECL_PRIVATE(McQmlSocket)
};

Q_DECLARE_METATYPE(McQmlSocket *)
