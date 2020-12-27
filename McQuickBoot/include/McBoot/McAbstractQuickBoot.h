#pragma once

#include "IMcQuickBoot.h"

MC_FORWARD_DECL_PRIVATE_DATA(McAbstractQuickBoot);

class MCQUICKBOOT_EXPORT McAbstractQuickBoot : public QObject, public IMcQuickBoot
{
    Q_OBJECT
    MC_DECL_INIT(McAbstractQuickBoot)
public:
    using QObject::connect;

    explicit McAbstractQuickBoot(QObject *parent = nullptr);
    ~McAbstractQuickBoot() override;

    QMetaObject::Connection connect(const QString &sender,
                                    const QString &signal,
                                    const QString &receiver,
                                    const QString &slot,
                                    Qt::ConnectionType type = Qt::AutoConnection) noexcept;

private:
    MC_DECL_PRIVATE(McAbstractQuickBoot)
};

MC_DECL_POINTER(McAbstractQuickBoot);
