#include "McBoot/McAbstractQuickBoot.h"

MC_DECL_PRIVATE_DATA(McAbstractQuickBoot)
MC_DECL_PRIVATE_DATA_END

MC_INIT(McAbstractQuickBoot)
MC_INIT_END

McAbstractQuickBoot::McAbstractQuickBoot(QObject *parent) : QObject(parent)
{
    MC_NEW_PRIVATE_DATA(McAbstractQuickBoot);
}

McAbstractQuickBoot::~McAbstractQuickBoot() {}

QMetaObject::Connection McAbstractQuickBoot::connect(const QString &sender,
                                                     const QString &signal,
                                                     const QString &receiver,
                                                     const QString &slot,
                                                     Qt::ConnectionType type) noexcept
{
    return Mc::Ioc::connect(getApplicationContext(), sender, signal, receiver, slot, type);
}
