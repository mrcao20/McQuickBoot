#pragma once

#include "McAbstractBeanDefinitionReader.h"

MC_FORWARD_DECL_CLASS(IMcBeanDefinition);

MC_FORWARD_DECL_PRIVATE_DATA(McAnnotationBeanDefinitionReader);

class MCIOC_EXPORT McAnnotationBeanDefinitionReader 
        : public McAbstractBeanDefinitionReader 
{
    Q_OBJECT
public:
    McAnnotationBeanDefinitionReader(
            const QHash<QString, IMcBeanDefinitionPtr> &definitions
            , QObject *parent = nullptr);
    ~McAnnotationBeanDefinitionReader() override;

protected:
    void doReadBeanDefinition() noexcept override;
    
private:
    void inject(const QString &beanName,
                IMcBeanDefinitionConstPtrRef beanDefinition) noexcept;
    void injectProperty(const QMetaObject *metaObj,
                        IMcBeanDefinitionConstPtrRef beanDefinition) noexcept;
    
private:
    MC_DECL_PRIVATE(McAnnotationBeanDefinitionReader)
};

MC_DECL_POINTER(McAnnotationBeanDefinitionReader)
