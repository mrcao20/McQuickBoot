#pragma once

#include "../../McBootGlobal.h"

MC_FORWARD_DECL_PRIVATE_DATA(McInnerSocket);

MC_FORWARD_DECL_CLASS(IMcSession);

class MCIOCBOOT_EXPORT McInnerSocket : public QObject 
{
    Q_OBJECT
public:
    explicit McInnerSocket(QObject *parent = nullptr);
    ~McInnerSocket() override;

    void setInnerSocketObj(QObjectConstPtrRef obj) noexcept;
    
    void setOnOpen(const QMetaMethod &method) noexcept;
    void setOnClose(const QMetaMethod &method) noexcept;
    void setOnError(const QMetaMethod &method) noexcept;
    void setOnMessage(const QMetaMethod &method) noexcept;
    
    void opened(IMcSessionConstPtrRef session) noexcept;
    void closed(IMcSessionConstPtrRef session) noexcept;
    void errored(IMcSessionConstPtrRef session, const QString &errMsg) noexcept;
    void receivedMsg(IMcSessionConstPtrRef session, const QVariant &msg) noexcept;
    
private:
    void invoke(const QMetaMethod &method
                , const QVector<QGenericArgument> &args) noexcept;
    
private:
    MC_DECL_PRIVATE(McInnerSocket)
};

MC_DECL_POINTER(McInnerSocket)
