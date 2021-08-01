#include "McBoot/Utils/Response/McResponseHandlerFactory.h"

MC_GLOBAL_STATIC_BEGIN(staticData)
QList<IMcResponseHandlerPtr> handlers;
MC_GLOBAL_STATIC_END(staticData)

QList<IMcResponseHandlerPtr> McResponseHandlerFactory::getHandlers() noexcept
{
    return staticData->handlers;
}

void McResponseHandlerFactory::addHanlder(const IMcResponseHandlerPtr &val) noexcept
{
    staticData->handlers.append(val);
}
