#include "McBoot/Configuration/McLogConfig.h"

#include <McLog/Configurator/McXMLConfigurator.h>

MC_STATIC()
MC_REGISTER_BEAN_FACTORY(McLogConfig)
MC_STATIC_END

MC_DECL_PRIVATE_DATA(McLogConfig)
QString xmlPath;
QString flag;
MC_DECL_PRIVATE_DATA_END

McLogConfig::McLogConfig(QObject *parent) noexcept : QObject(parent)
{
    MC_NEW_PRIVATE_DATA(McLogConfig);
}

McLogConfig::~McLogConfig() {}

void McLogConfig::allFinished() noexcept
{
    if (d->xmlPath.isEmpty()) {
        return;
    }
    McXMLConfigurator::configure(d->xmlPath, d->flag);
}

#include "moc_McLogConfig.cpp"
