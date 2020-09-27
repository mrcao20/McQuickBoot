#pragma once

#include "McAbstractBeanDefinitionReader.h"

QT_BEGIN_NAMESPACE
MC_FORWARD_DECL_CLASS(QSettings);
QT_END_NAMESPACE

MC_FORWARD_DECL_CLASS(IMcBeanDefinition);

MC_FORWARD_DECL_PRIVATE_DATA(McSettingBeanDefinitionReader);

class MCIOCCONTAINER_EXPORT McSettingBeanDefinitionReader : public McAbstractBeanDefinitionReader 
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
    void readBeanDefinitionForProperty(QSettingsConstPtrRef setting
                                       , IMcBeanDefinitionConstPtrRef beanDefinition) noexcept;
    void readBeanDefinitionForConnect(QSettingsConstPtrRef setting
                                      , IMcBeanDefinitionConstPtrRef beanDefinition) noexcept;
    QVariant parseProperty(const QVariant &var) noexcept;

private:
	MC_DECL_PRIVATE(McSettingBeanDefinitionReader)
};

MC_DECL_POINTER(McSettingBeanDefinitionReader)
