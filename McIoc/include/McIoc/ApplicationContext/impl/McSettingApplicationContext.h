#pragma once

#include "McReadableApplicationContext.h"

#include <QSettings>

QT_BEGIN_NAMESPACE
MC_FORWARD_DECL_CLASS(QSettings);
QT_END_NAMESPACE

MC_FORWARD_DECL_CLASS(IMcBeanDefinitionReader);

class MCIOC_EXPORT McSettingApplicationContext 
        : public McReadableApplicationContext 
{
    Q_OBJECT
public:
    explicit McSettingApplicationContext(QObject *parent = nullptr);
    McSettingApplicationContext(QSettingsConstPtrRef setting, QObject *parent = nullptr);
    McSettingApplicationContext(const QList<QSettingsPtr> &settings, QObject *parent = nullptr);
    McSettingApplicationContext(QSettings::Format format
                                , const QString &location
                                , QObject *parent = nullptr);
    McSettingApplicationContext(QSettings::Format format
                                , const QStringList &locations
                                , QObject *parent = nullptr);
    McSettingApplicationContext(IMcBeanDefinitionReaderConstPtrRef reader
                            , QObject *parent = nullptr);
    McSettingApplicationContext(IMcConfigurableBeanFactoryConstPtrRef factory
                            , IMcBeanDefinitionReaderConstPtrRef reader
                            , QObject *parent = nullptr);
    ~McSettingApplicationContext() override;
    
    void setSetting(QSettingsConstPtrRef setting) noexcept;
    void setSettings(const QList<QSettingsPtr> &settings) noexcept;
};

MC_DECL_POINTER(McSettingApplicationContext)
