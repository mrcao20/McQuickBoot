#include "McIoc/ApplicationContext/impl/McSettingApplicationContext.h"

#include <QFile>
#include <QDebug>

#include "McIoc/BeanDefinitionReader/impl/McSettingBeanDefinitionReader.h"

McSettingApplicationContext::McSettingApplicationContext(QObject *parent)
    : McReadableApplicationContext(parent)
{
}

McSettingApplicationContext::McSettingApplicationContext(
        QSettingsConstPtrRef setting, QObject *parent)
    : McSettingApplicationContext(QList<QSettingsPtr>() << setting, parent)
{
}

McSettingApplicationContext::McSettingApplicationContext(
        const QList<QSettingsPtr> &settings, QObject *parent)
    : McSettingApplicationContext(parent)
{
    setSettings(settings);
}

McSettingApplicationContext::McSettingApplicationContext(
        QSettings::Format format, const QString &location, QObject *parent)
    : McSettingApplicationContext(format, QStringList() << location, parent)
{
}

McSettingApplicationContext::McSettingApplicationContext(
        QSettings::Format format, const QStringList &locations, QObject *parent)
    : McSettingApplicationContext(parent)
{
    QList<QSettingsPtr> settings;
    for(auto location : locations) {
        if(!QFile::exists(location)) {
            qCritical() << "file '" << location << "' not exists";
            continue;
        }
        
        QSettings *setting = new QSettings(location, format);
        if (setting->status() != QSettings::Status::NoError) {
            qCritical() << "create yaml setting failure. status:"
                        << setting->status();
            continue;
        }
        settings.append(QSettingsPtr(setting));
    }
    setSettings(settings);
}

McSettingApplicationContext::McSettingApplicationContext(
        IMcBeanDefinitionReaderConstPtrRef reader, QObject *parent)
    : McReadableApplicationContext(reader, parent)
{
}

McSettingApplicationContext::McSettingApplicationContext(
        IMcConfigurableBeanFactoryConstPtrRef factory,
        IMcBeanDefinitionReaderConstPtrRef reader,
        QObject *parent)
    : McReadableApplicationContext(factory, reader, parent)
{
}

McSettingApplicationContext::~McSettingApplicationContext()
{
}

void McSettingApplicationContext::setSetting(QSettingsConstPtrRef setting) noexcept 
{
    setSettings(QList<QSettingsPtr>() << setting);
}

void McSettingApplicationContext::setSettings(const QList<QSettingsPtr> &settings) noexcept 
{
    setReader(McSettingBeanDefinitionReaderPtr::create(settings));
}
