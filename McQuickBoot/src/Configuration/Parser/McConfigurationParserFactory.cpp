#include "McBoot/Configuration/Parser/McConfigurationParserFactory.h"

MC_GLOBAL_STATIC_BEGIN(staticData)
QList<IMcConfigurationParserPtr> parsers;
MC_GLOBAL_STATIC_END(staticData)

QList<IMcConfigurationParserPtr> McConfigurationParserFactory::getParsers() noexcept
{
    return staticData->parsers;
}

void McConfigurationParserFactory::registerParser(const IMcConfigurationParserPtr &val) noexcept
{
    staticData->parsers.append(val);
}
