#pragma once

#include "McAbstractConnection.h"

MC_FORWARD_DECL_PRIVATE_DATA(McCppConnection);

class McCppConnection : public McAbstractConnection
{
public:
    McCppConnection(QObject *parent = nullptr) noexcept;
    ~McCppConnection() override;

    QMetaObject::Connection init(QObject *sender,
                                 const QString &signal,
                                 QObject *receiver,
                                 QtPrivate::QSlotObjectBase *slot,
                                 Qt::ConnectionType type) noexcept;

protected:
    using McAbstractConnection::init;
    void call(void **arguments) noexcept override;

private:
    MC_DECL_PRIVATE(McCppConnection)
};
