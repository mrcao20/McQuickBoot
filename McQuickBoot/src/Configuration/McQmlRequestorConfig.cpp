#include "McBoot/Configuration/McQmlRequestorConfig.h"

MC_INIT(McQmlRequestorConfig)
MC_REGISTER_BEAN_FACTORY(McQmlRequestorConfig)
MC_INIT_END

MC_DECL_PRIVATE_DATA(McQmlRequestorConfig)
int maxThreadCount{10};
MC_DECL_PRIVATE_DATA_END

McQmlRequestorConfig::McQmlRequestorConfig(QObject *parent) noexcept
    : QObject(parent)
{
    MC_NEW_PRIVATE_DATA(McQmlRequestorConfig);
}

McQmlRequestorConfig::~McQmlRequestorConfig()
{
}

int McQmlRequestorConfig::maxThreadCount() const noexcept
{
    return d->maxThreadCount;
}

void McQmlRequestorConfig::setMaxThreadCount(int val) noexcept
{
    d->maxThreadCount = val;
}
