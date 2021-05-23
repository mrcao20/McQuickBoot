#include "McIoc/ApplicationContext/impl/McAbstractApplicationContext.h"

#include "McIoc/BeanFactory/IMcBeanFactory.h"
#include "McIoc/BeanDefinition/IMcBeanDefinition.h"

MC_DECL_PRIVATE_DATA(McAbstractApplicationContext)
IMcConfigurableBeanFactoryPtr configurableBeanFactory;
QList<IMcConfigurableBeanFactoryPtr> relatedBeanFactory;
MC_DECL_PRIVATE_DATA_END

McAbstractApplicationContext::McAbstractApplicationContext(
    IMcConfigurableBeanFactoryConstPtrRef factory, QObject *parent)
    : QObject(parent)
{
    MC_NEW_PRIVATE_DATA(McAbstractApplicationContext)
    d->configurableBeanFactory = factory;
}

McAbstractApplicationContext::~McAbstractApplicationContext()
{
}

IMcPropertyConverterPtr McAbstractApplicationContext::getPropertyConverter() const noexcept
{
    return d->configurableBeanFactory->getPropertyConverter();
}

void McAbstractApplicationContext::setPropertyConverter(
    IMcPropertyConverterConstPtrRef converter) noexcept
{
    d->configurableBeanFactory->setPropertyConverter(converter);
}

QObjectPtr McAbstractApplicationContext::getBean(const QString &name, QThread *thread) noexcept
{
    auto var = getBeanToVariant(name, thread);
    if(!var.isValid())
        return QObjectPtr();
    return var.value<QObjectPtr>();
}

QObject *McAbstractApplicationContext::getBeanPointer(const QString &name, QThread *thread) noexcept
{
    auto var = getBeanToVariant(name, thread);
    if (!var.isValid())
        return nullptr;
    return var.value<QObject *>();
}

QVariant McAbstractApplicationContext::getBeanToVariant(const QString &name,
                                                        QThread *thread) noexcept
{
    QVariant var;
    if(d->configurableBeanFactory->containsBean(name)) {
        var = d->configurableBeanFactory->getBeanToVariant(name, thread);
    } else {
        for (auto beanFactory : d->relatedBeanFactory) {
            if (beanFactory->containsBean(name)) {
                var = beanFactory->getBeanToVariant(name, thread);
                break;
            }
        }
    }
    return var;
}

bool McAbstractApplicationContext::containsBean(const QString &name) const noexcept
{
    bool isContained = false;
    if(d->configurableBeanFactory->containsBean(name)) {
        isContained = true;
    } else {
        for (auto beanFactory : d->relatedBeanFactory) {
            if (beanFactory->containsBean(name)) {
                isContained = true;
                break;
            }
        }
    }
    return isContained;
}

bool McAbstractApplicationContext::isSingleton(const QString &name) noexcept 
{
    bool has = false;
    if(d->configurableBeanFactory->isSingleton(name)) {
        has = true;
    } else {
        for (auto beanFactory : d->relatedBeanFactory) {
            if (beanFactory->isSingleton(name)) {
                has = true;
                break;
            }
        }
    }
    return has;
}

bool McAbstractApplicationContext::registerBeanDefinition(
    const QString &name, IMcBeanDefinitionConstPtrRef beanDefinition) noexcept
{
    if (d->configurableBeanFactory->canRegister(beanDefinition)) {
        d->configurableBeanFactory->registerBeanDefinition(name, beanDefinition);
        return true;
    }
    for (auto beanFactory : d->relatedBeanFactory) {
        if (beanFactory->canRegister(beanDefinition)) {
            beanFactory->registerBeanDefinition(name, beanDefinition);
            return true;
        }
    }
    return false;
}

IMcBeanDefinitionPtr McAbstractApplicationContext::unregisterBeanDefinition(const QString &name) noexcept
{
    if(d->configurableBeanFactory->containsBean(name)) {
        return d->configurableBeanFactory->unregisterBeanDefinition(name);
    } else {
        for (auto beanFactory : d->relatedBeanFactory) {
            if (beanFactory->containsBean(name)) {
                return beanFactory->unregisterBeanDefinition(name);
            }
        }
    }
    return IMcBeanDefinitionPtr();
}

bool McAbstractApplicationContext::canRegister(IMcBeanDefinitionConstPtrRef beanDefinition) const
    noexcept
{
    if (d->configurableBeanFactory->canRegister(beanDefinition)) {
        return true;
    }
    for (auto beanFactory : d->relatedBeanFactory) {
        if (beanFactory->canRegister(beanDefinition)) {
            return true;
        }
    }
    return false;
}

bool McAbstractApplicationContext::isContained(const QString &name) const noexcept
{
    return containsBean(name);
}

QHash<QString, IMcBeanDefinitionPtr> McAbstractApplicationContext::getBeanDefinitions() const
    noexcept
{
    auto beanDefinitions = d->configurableBeanFactory->getBeanDefinitions();
    for (auto beanFactory : d->relatedBeanFactory) {
        auto child = beanFactory->getBeanDefinitions();
        for(auto key : child.keys()) {
            beanDefinitions.insert(key, child.value(key));
        }
    }
    return beanDefinitions;
}

void McAbstractApplicationContext::refresh(QThread *thread) noexcept 
{
    auto beanDefinitions = getBeanDefinitions();
    auto beanNames = beanDefinitions.keys();    //!< 获取所有beanName
    for(auto beanName : beanNames) {
        auto beanDefinition = beanDefinitions.value(beanName);
        if(beanDefinition->isSingleton()) {         //!< 如果不是单例对象则不需要获取，因为非单例对象无法预存
            getBeanToVariant(beanName, thread);                      //!< 获取一次bean，让bean预加载
        }
    }
}

void McAbstractApplicationContext::addRelatedBeanFactory(
    IMcConfigurableBeanFactoryConstPtrRef beanFac) noexcept
{
    if (d->relatedBeanFactory.contains(beanFac)) {
        return;
    }
    d->relatedBeanFactory.append(beanFac);
}

void McAbstractApplicationContext::removeRelatedBeanFactory(
    IMcConfigurableBeanFactoryConstPtrRef beanFac) noexcept
{
    if (!d->relatedBeanFactory.contains(beanFac)) {
        return;
    }
    d->relatedBeanFactory.removeAll(beanFac);
}

QList<IMcConfigurableBeanFactoryPtr> McAbstractApplicationContext::getRelatedBeanFactories() noexcept
{
    return d->relatedBeanFactory;
}
