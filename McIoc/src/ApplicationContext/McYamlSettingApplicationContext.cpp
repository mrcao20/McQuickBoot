#include "McIoc/ApplicationContext/impl/McYamlSettingApplicationContext.h"

#include "McIoc/Utils/McYaml.h"

MC_INIT(McYamlSettingApplicationContext)
MC_INIT_END

McYamlSettingApplicationContext::McYamlSettingApplicationContext(QObject *parent)
    : McSettingApplicationContext(parent)
{
}

McYamlSettingApplicationContext::McYamlSettingApplicationContext(
        const QString &location, QObject *parent)
    : McYamlSettingApplicationContext(QStringList() << location, parent)
{
}

McYamlSettingApplicationContext::McYamlSettingApplicationContext(
        const QStringList &locations, QObject *parent)
    : McSettingApplicationContext(McYaml::format(), locations, parent)
{
}
