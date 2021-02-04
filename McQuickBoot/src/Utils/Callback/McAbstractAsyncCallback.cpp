#include "McBoot/Utils/Callback/Impl/McAbstractAsyncCallback.h"

void McAbstractAsyncCallback::destroy() noexcept
{
    deleteLater();
}
