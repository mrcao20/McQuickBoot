#pragma once

#include <QObject>

#include <McIoc/McGlobal.h>

MC_FORWARD_DECL_PRIVATE_DATA(McAbstractConnection);

class McAbstractConnection : public QObject
{
public:
    McAbstractConnection(QObject *parent = nullptr) noexcept;
    ~McAbstractConnection() override;

    int qt_metacall(QMetaObject::Call c, int id, void **arguments) override final;

protected:
    virtual void call(void **arguments) noexcept = 0;

protected:
    QMetaObject::Connection init(QObject *obj,
                                 const QString &signal,
                                 Qt::ConnectionType type,
                                 QMetaMethod &sig) noexcept;

private:
    MC_DECL_PRIVATE(McAbstractConnection)
};
