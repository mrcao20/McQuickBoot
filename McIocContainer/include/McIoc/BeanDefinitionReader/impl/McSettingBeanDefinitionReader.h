#pragma once

#include "McAbstractBeanDefinitionReader.h"

QT_BEGIN_NAMESPACE
MC_FORWARD_DECL_CLASS(QSettings);
QT_END_NAMESPACE

MC_FORWARD_DECL_CLASS(IMcBeanDefinition)

MC_FORWARD_DECL_PRIVATE_DATA(McSettingBeanDefinitionReader)

class MCIOCCONTAINER_EXPORT McSettingBeanDefinitionReader : public McAbstractBeanDefinitionReader 
{
    Q_OBJECT
    using McAbstractBeanDefinitionReader::readBeanDefinition;
public:
    explicit McSettingBeanDefinitionReader(
        QSettingsConstPtrRef settings
        , QObject *parent = nullptr);
    virtual ~McSettingBeanDefinitionReader() override;

protected:
    void doReadBeanDefinition() noexcept override;

private:
    Qt::ConnectionType getConnectionType(const QString &typeStr) noexcept;
    Qt::ConnectionType connectionTypeStrToEnum(const QString &typeStr) noexcept;

private:
	MC_DECL_PRIVATE(McSettingBeanDefinitionReader)
};

MC_DECL_POINTER(McSettingBeanDefinitionReader)
