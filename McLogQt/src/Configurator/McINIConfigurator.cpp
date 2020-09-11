#include "McLog/Configurator/McINIConfigurator.h"

#include <QCoreApplication>
#include <QSettings>
#include <QDir>

McINIConfigurator::McINIConfigurator() {
}


void McINIConfigurator::configure(const QString &path) noexcept 
{
    auto iniPath = QDir::toNativeSeparators(path);
    QString sep = ".";
    sep += QDir::separator();
    if(iniPath.startsWith(sep)) {
        iniPath.remove(0, 1);
        iniPath = qApp->applicationDirPath() + iniPath;
    }
    QSettings settings(iniPath, QSettings::Format::IniFormat);
    McSettingConfigurator::configure(settings);
}
