#pragma once

#include "McAbstractBeanDefinitionReader.h"

QT_BEGIN_NAMESPACE
MC_FORWARD_DECL_CLASS(QSettings);
QT_END_NAMESPACE

MC_FORWARD_DECL_CLASS(IMcBeanDefinition);

MC_FORWARD_DECL_PRIVATE_DATA(McSettingBeanDefinitionReader);

class MCIOC_EXPORT McSettingBeanDefinitionReader : public McAbstractBeanDefinitionReader 
{
    Q_OBJECT
    using McAbstractBeanDefinitionReader::readBeanDefinition;
public:
    explicit McSettingBeanDefinitionReader(
        QSettingsConstPtrRef setting, QObject *parent = nullptr);
    explicit McSettingBeanDefinitionReader(
        const QList<QSettingsPtr> &settings, QObject *parent = nullptr);
    virtual ~McSettingBeanDefinitionReader() override;

protected:
    void doReadBeanDefinition() noexcept override;
    
private:
    IMcBeanDefinitionPtr buildBeanDefinition(QSettingsConstPtrRef setting) noexcept;
    void readBeanDefinitionForProperty(QSettingsConstPtrRef setting,
                                       IMcBeanDefinitionConstPtrRef beanDefinition) noexcept;
    void readBeanDefinitionForProperty(const QMap<QString, QVariant> &map,
                                       IMcBeanDefinitionConstPtrRef beanDefinition) noexcept;
    void readBeanDefinitionForConnect(QSettingsConstPtrRef setting,
                                      IMcBeanDefinitionConstPtrRef beanDefinition) noexcept;
    void readBeanDefinitionForConnect(const QMap<QString, QVariant> &map,
                                      IMcBeanDefinitionConstPtrRef beanDefinition) noexcept;
    QVariant parseProperty(const QVariant &var) noexcept;
    QString getUnregisteredBeanName(const QString &proName, const QString &className) const noexcept;

private:
	MC_DECL_PRIVATE(McSettingBeanDefinitionReader)
};

MC_DECL_POINTER(McSettingBeanDefinitionReader)
