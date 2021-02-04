#pragma once

#include <QObject>

#include <QJSValue>

#include <McIoc/McGlobal.h>

MC_FORWARD_DECL_PRIVATE_DATA(McQmlConnection);

class QJSEngine;

class McQmlConnection : public QObject
{
public:
    McQmlConnection(QObject *parent = nullptr) noexcept;
    ~McQmlConnection() override;

    bool init(QObject *obj,
              const QString &signal,
              const QJSValue &callback,
              QJSEngine *jsEngine) noexcept;

    bool check(QObject *obj, const QString &signal, const QJSValue &callback) noexcept;

    int qt_metacall(QMetaObject::Call c, int id, void **arguments) override;

private:
    MC_DECL_PRIVATE(McQmlConnection)
};
