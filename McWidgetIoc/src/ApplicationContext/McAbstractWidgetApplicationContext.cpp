#include "McWidgetIoc/ApplicationContext/Impl/McAbstractWidgetApplicationContext.h"

MC_DECL_PRIVATE_DATA(McAbstractWidgetApplicationContext)
IMcConfigurableWidgetBeanFactoryPtr configurableBeanFactory;
MC_DECL_PRIVATE_DATA_END

McAbstractWidgetApplicationContext::McAbstractWidgetApplicationContext(QObject *parent)
    : QObject(parent)
{
    MC_NEW_PRIVATE_DATA(McAbstractWidgetApplicationContext);
}

McAbstractWidgetApplicationContext::~McAbstractWidgetApplicationContext() {}

QWidget *McAbstractWidgetApplicationContext::getBean(const QString &name, QWidget *parent) const
    noexcept
{
    return d->configurableBeanFactory->getBean(name, parent);
}

bool McAbstractWidgetApplicationContext::containsBean(const QString &name) const noexcept
{
    return d->configurableBeanFactory->containsBean(name);
}

bool McAbstractWidgetApplicationContext::registerBeanDefinition(
    const QString &name, IMcBeanDefinitionConstPtrRef beanDefinition) noexcept
{
    return d->configurableBeanFactory->registerBeanDefinition(name, beanDefinition);
}

IMcBeanDefinitionPtr McAbstractWidgetApplicationContext::unregisterBeanDefinition(
    const QString &name) noexcept
{
    return d->configurableBeanFactory->unregisterBeanDefinition(name);
}

bool McAbstractWidgetApplicationContext::canRegister(
    IMcBeanDefinitionConstPtrRef beanDefinition) const noexcept
{
    return d->configurableBeanFactory->canRegister(beanDefinition);
}

bool McAbstractWidgetApplicationContext::isContained(const QString &name) const noexcept
{
    return containsBean(name);
}

QHash<QString, IMcBeanDefinitionPtr> McAbstractWidgetApplicationContext::getBeanDefinitions() const
    noexcept
{
    return d->configurableBeanFactory->getBeanDefinitions();
}

void McAbstractWidgetApplicationContext::setBeanFactory(
    IMcConfigurableWidgetBeanFactoryConstPtrRef factory) noexcept
{
    d->configurableBeanFactory = factory;
}
