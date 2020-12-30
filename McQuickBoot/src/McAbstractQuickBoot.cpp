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
