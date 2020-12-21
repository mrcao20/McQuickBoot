#include "McIoc/ApplicationContext/impl/McIniSettingApplicationContext.h"

McIniSettingApplicationContext::McIniSettingApplicationContext(QObject *parent)
    : McSettingApplicationContext(parent)
{
}

McIniSettingApplicationContext::McIniSettingApplicationContext(const QString &location,
                                                               QObject *parent)
    : McIniSettingApplicationContext(QStringList() << location, parent)
{
}

McIniSettingApplicationContext::McIniSettingApplicationContext(const QStringList &locations,
                                                               QObject *parent)
    : McSettingApplicationContext(QSettings::IniFormat, locations, parent)
{
}
