#include "McBoot/Utils/McCancel.h"

MC_INIT(McCancel)
qRegisterMetaType<McCancel>();
MC_INIT_END

McCancel::McCancel() noexcept
{
    d = new McCancelSharedData();
}

McCancel::~McCancel() {}

bool McCancel::isCanceled() const noexcept
{
    return d->isCanceled.loadAcquire();
}

void McCancel::cancel() noexcept
{
    d->isCanceled.storeRelease(true);
}
