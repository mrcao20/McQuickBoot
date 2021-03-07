#pragma once

#include "McAbstractConnection.h"

#include <QJSValue>

MC_FORWARD_DECL_PRIVATE_DATA(McQmlConnection);

class QJSEngine;

class McQmlConnection : public McAbstractConnection
{
public:
    McQmlConnection(QObject *parent = nullptr) noexcept;
    ~McQmlConnection() override;

    bool init(QObject *obj,
              const QString &signal,
              const QJSValue &callback,
              QJSEngine *jsEngine) noexcept;

    bool check(QObject *obj, const QString &signal, const QJSValue &callback) noexcept;

protected:
    using McAbstractConnection::init;
    void call(void **arguments) noexcept override;

private:
    MC_DECL_PRIVATE(McQmlConnection)
};
