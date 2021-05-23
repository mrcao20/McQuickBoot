#pragma once

#include "../IMcConfigurableApplicationContext.h"

MC_FORWARD_DECL_PRIVATE_DATA(McAbstractApplicationContext);

class MCIOC_EXPORT McAbstractApplicationContext : public QObject,
                                                  public IMcConfigurableApplicationContext
{
    Q_OBJECT
public:
    using IMcBeanFactory::getBean;
    using IMcBeanFactory::getBeanPointer;

    McAbstractApplicationContext(IMcConfigurableBeanFactoryConstPtrRef factory,
                                 QObject *parent = nullptr);
    ~McAbstractApplicationContext() override;

    IMcPropertyConverterPtr getPropertyConverter() const noexcept override;
    void setPropertyConverter(IMcPropertyConverterConstPtrRef converter) noexcept override;

    QObjectPtr getBean(const QString &name, QThread *thread = nullptr) noexcept override;
    QObject *getBeanPointer(const QString &name, QThread *thread = nullptr) noexcept override;
    QVariant getBeanToVariant(const QString &name, QThread *thread = nullptr)  noexcept override;
    bool containsBean(const QString &name) const noexcept override;
    bool isSingleton(const QString &name) noexcept override;
    bool registerBeanDefinition(const QString &name,
                                IMcBeanDefinitionConstPtrRef beanDefinition) noexcept override;
    IMcBeanDefinitionPtr unregisterBeanDefinition(const QString &name) noexcept override;
    bool canRegister(IMcBeanDefinitionConstPtrRef beanDefinition) const noexcept override;
    bool isContained(const QString &name) const noexcept override;
    QHash<QString, IMcBeanDefinitionPtr> getBeanDefinitions() const noexcept override;
    void refresh(QThread *thread = nullptr) noexcept override;
    
    void addRelatedBeanFactory(IMcConfigurableBeanFactoryConstPtrRef beanFac) noexcept override;
    void removeRelatedBeanFactory(IMcConfigurableBeanFactoryConstPtrRef beanFac) noexcept override;
    QList<IMcConfigurableBeanFactoryPtr> getRelatedBeanFactories() noexcept override;

private:
    MC_DECL_PRIVATE(McAbstractApplicationContext)
};

MC_DECL_POINTER(McAbstractApplicationContext)
