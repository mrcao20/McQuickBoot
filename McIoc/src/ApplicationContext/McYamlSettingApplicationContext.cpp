#include "McIoc/ApplicationContext/impl/McYamlSettingApplicationContext.h"

#include <McYaml/McYaml.h>

MC_INIT(McYamlSettingApplicationContext)
McYaml::registerYaml();
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
