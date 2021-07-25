#include "McLog/Configurator/McINIConfigurator.h"

#include <QCoreApplication>
#include <QSettings>
#include <QDir>

#include <McIoc/McGlobal.h>

McINIConfigurator::McINIConfigurator() {
}


void McINIConfigurator::configure(const QString &path) noexcept 
{
    auto iniPath = Mc::toAbsolutePath(path);
    QSettings settings(iniPath, QSettings::Format::IniFormat);
    McSettingConfigurator::configure(settings);
}
