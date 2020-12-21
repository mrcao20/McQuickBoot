#include "McBoot/Configuration/McRequestorConfig.h"

MC_INIT(McRequestorConfig)
MC_REGISTER_BEAN_FACTORY(McRequestorConfig)
MC_INIT_END

MC_DECL_PRIVATE_DATA(McRequestorConfig)
int maxThreadCount{10};
MC_DECL_PRIVATE_DATA_END

McRequestorConfig::McRequestorConfig(QObject *parent) noexcept : QObject(parent)
{
    MC_NEW_PRIVATE_DATA(McRequestorConfig);
}

McRequestorConfig::~McRequestorConfig() {}

int McRequestorConfig::maxThreadCount() const noexcept
{
    return d->maxThreadCount;
}

void McRequestorConfig::setMaxThreadCount(int val) noexcept
{
    d->maxThreadCount = val;
}
