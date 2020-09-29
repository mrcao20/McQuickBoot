#pragma once

#include <McIoc/BeanDefinitionReader/impl/McAbstractBeanDefinitionReader.h>

MC_FORWARD_DECL_CLASS(IMcBeanDefinition);
MC_FORWARD_DECL_CLASS(IMcApplicationContext);
MC_FORWARD_DECL_CLASS(QSettings);

MC_FORWARD_DECL_PRIVATE_DATA(McConfigurationFileBeanDefinitionReader);

class McConfigurationFileBeanDefinitionReader 
        : public McAbstractBeanDefinitionReader 
{
    Q_OBJECT
public:
    McConfigurationFileBeanDefinitionReader(
            IMcApplicationContextConstPtrRef appCtx
            , QObject *parent = nullptr);
    ~McConfigurationFileBeanDefinitionReader() override;

protected:
    void doReadBeanDefinition() noexcept override;
    
private:
    QString getDefaultConfigPath() const noexcept;
    void copyBeanDefinition(QSettingsConstPtrRef setting
                            , const QString &path, const QString &prefix) noexcept;
    void copyBeanDefinition(QSettingsConstPtrRef setting, QSettings &originSet) noexcept;
    
private:
    MC_DECL_PRIVATE(McConfigurationFileBeanDefinitionReader)
};

MC_DECL_POINTER(McConfigurationFileBeanDefinitionReader)
