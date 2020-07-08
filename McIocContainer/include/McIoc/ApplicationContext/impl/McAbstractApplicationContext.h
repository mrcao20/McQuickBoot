#pragma once

#include "../IMcApplicationContext.h"

MC_FORWARD_DECL_PRIVATE_DATA(McAbstractApplicationContext);

class MCIOCCONTAINER_EXPORT McAbstractApplicationContext
        : public QObject
        , public IMcApplicationContext 
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
    bool isContained(const QString &name) noexcept override;
    QHash<QString, IMcBeanDefinitionPtr> getBeanDefinitions() noexcept override;
    void refresh(QThread *thread = nullptr) noexcept override;
    
protected:
    virtual void doRefresh() noexcept = 0;
    
private:
    MC_DECL_PRIVATE(McAbstractApplicationContext)
};

MC_DECL_POINTER(McAbstractApplicationContext)
