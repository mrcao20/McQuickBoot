#pragma once

#include "../IMcConfigurableApplicationContext.h"

MC_FORWARD_DECL_PRIVATE_DATA(McAbstractApplicationContext);

class MCIOCCONTAINER_EXPORT McAbstractApplicationContext
        : public QObject
        , public IMcConfigurableApplicationContext 
{
                     
    Q_OBJECT
public:
    using IMcBeanFactory::getBean;
    
    McAbstractApplicationContext(IMcConfigurableBeanFactoryConstPtrRef factory
                                 , QObject *parent = nullptr);
    ~McAbstractApplicationContext() override;

    QObjectPtr getBean(const QString &name, QThread *thread = nullptr) noexcept override;
    QVariant getBeanToVariant(const QString &name, QThread *thread = nullptr)  noexcept override;
    bool containsBean(const QString &name) noexcept override;
    bool isSingleton(const QString &name) noexcept override;
    void registerBeanDefinition(const QString &name
                                , IMcBeanDefinitionConstPtrRef beanDefinition) noexcept override;
    IMcBeanDefinitionPtr unregisterBeanDefinition(const QString &name) noexcept override;
    bool isContained(const QString &name) noexcept override;
    QHash<QString, IMcBeanDefinitionPtr> getBeanDefinitions() noexcept override;
    void refresh(QThread *thread = nullptr) noexcept override;
    
    QVariant getBeanSelf(const QString &name, QThread *thread = nullptr) noexcept override;
    bool containsBeanSelf(const QString &name) noexcept override;
    bool isSingletonSelf(const QString &name) noexcept override;
    void registerBeanDefinitionSelf(
            const QString &name
            , IMcBeanDefinitionConstPtrRef beanDefinition) noexcept override;
    IMcBeanDefinitionPtr unregisterBeanDefinitionSelf(const QString &name) noexcept override;
    QHash<QString, IMcBeanDefinitionPtr> getBeanDefinitionsSelf() noexcept override;
    
    void addRelatedApplicationContext(IMcApplicationContextConstPtrRef appCtx) noexcept override;
    void removeRelatedApplicationContext(IMcApplicationContextConstPtrRef appCtx) noexcept override;
    QList<IMcApplicationContextPtr> getRelatedApplicationContexts() noexcept override;
    
protected:
    virtual void doRefresh() noexcept = 0;
    
private:
    MC_DECL_PRIVATE(McAbstractApplicationContext)
};

MC_DECL_POINTER(McAbstractApplicationContext)
