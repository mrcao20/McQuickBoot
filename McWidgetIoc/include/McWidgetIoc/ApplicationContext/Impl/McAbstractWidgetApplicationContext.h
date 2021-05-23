#pragma once

#include "../IMcWidgetApplicationContext.h"

MC_FORWARD_DECL_PRIVATE_DATA(McAbstractWidgetApplicationContext);

class MCWIDGETIOC_EXPORT McAbstractWidgetApplicationContext : public QObject,
                                                              public IMcWidgetApplicationContext
{
    Q_OBJECT
public:
    using IMcWidgetBeanFactory::getBean;

    McAbstractWidgetApplicationContext(QObject *parent = nullptr);
    ~McAbstractWidgetApplicationContext() override;

    QWidget *getBean(const QString &name, QWidget *parent = nullptr) const noexcept override;
    bool containsBean(const QString &name) const noexcept override;
    bool registerBeanDefinition(const QString &name,
                                IMcBeanDefinitionConstPtrRef beanDefinition) noexcept override;
    IMcBeanDefinitionPtr unregisterBeanDefinition(const QString &name) noexcept override;
    bool canRegister(IMcBeanDefinitionConstPtrRef beanDefinition) const noexcept override;
    bool isContained(const QString &name) const noexcept override;
    QHash<QString, IMcBeanDefinitionPtr> getBeanDefinitions() const noexcept override;

protected:
    void setBeanFactory(IMcConfigurableWidgetBeanFactoryConstPtrRef factory) noexcept;

private:
    MC_DECL_PRIVATE(McAbstractWidgetApplicationContext)
};

MC_DECL_POINTER(McAbstractWidgetApplicationContext)
