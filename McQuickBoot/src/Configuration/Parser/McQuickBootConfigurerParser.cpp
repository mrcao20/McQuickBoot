#include "McBoot/Configuration/Parser/Impl/McQuickBootConfigurerParser.h"

#include "McBoot/Configuration/IMcQuickBootConfigurer.h"
#include "McBoot/Configuration/Parser/McConfigurationParserFactory.h"

MC_INIT(McQuickBootConfigurerParser)
McConfigurationParserFactory::registerParser<McQuickBootConfigurerParser>();
MC_INIT_END

void McQuickBootConfigurerParser::parse(const QVariant &var) noexcept
{
    auto configurer = var.value<IMcQuickBootConfigurerPtr>();
    if (configurer.isNull()) {
        return;
    }
    auto registry = McResponseHandlerRegistry();
    configurer->addResponseHandler(registry);
}
