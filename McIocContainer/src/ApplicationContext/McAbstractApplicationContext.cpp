#include "McIoc/ApplicationContext/impl/McAbstractApplicationContext.h"

#include "McIoc/BeanFactory/IMcBeanFactory.h"
#include "McIoc/BeanDefinition/IMcBeanDefinition.h"

MC_DECL_PRIVATE_DATA(McAbstractApplicationContext)
IMcConfigurableBeanFactoryPtr configurableBeanFactory;
MC_DECL_PRIVATE_DATA_END

McAbstractApplicationContext::McAbstractApplicationContext(
        IMcConfigurableBeanFactoryConstPtrRef factory
        , QObject *parent)
    : QObject(parent)
{
    MC_NEW_PRIVATE_DATA(McAbstractApplicationContext)
    d->configurableBeanFactory = factory;
}

McAbstractApplicationContext::~McAbstractApplicationContext()
{}

QObjectPtr McAbstractApplicationContext::getBean(
        const QString &name, QThread *thread) noexcept 
{
    return d->configurableBeanFactory->getBean(name, thread);
}

QVariant McAbstractApplicationContext::getBeanToVariant(
        const QString &name, QThread *thread) noexcept 
{
    return d->configurableBeanFactory->getBeanToVariant(name, thread);
}

bool McAbstractApplicationContext::containsBean(const QString &name) noexcept
{
    return d->configurableBeanFactory->containsBean(name);
}

bool McAbstractApplicationContext::isSingleton(const QString &name) noexcept 
{
    return d->configurableBeanFactory->isSingleton(name);
}

void McAbstractApplicationContext::registerBeanDefinition(
        const QString &name
        , IMcBeanDefinitionConstPtrRef beanDefinition) noexcept 
{
    d->configurableBeanFactory->registerBeanDefinition(name, beanDefinition);
}

bool McAbstractApplicationContext::isContained(const QString &name) noexcept 
{
    return d->configurableBeanFactory->isContained(name);
}

QHash<QString, IMcBeanDefinitionPtr> McAbstractApplicationContext::getBeanDefinitions() noexcept 
{
    return d->configurableBeanFactory->getBeanDefinitions();
}

void McAbstractApplicationContext::refresh(QThread *thread) noexcept 
{
    doRefresh();
    
    auto beanDefinitions = getBeanDefinitions();
    auto beanNames = beanDefinitions.keys();    //!< 获取所有beanName
    for(auto beanName : beanNames) {
        auto beanDefinition = beanDefinitions.value(beanName);
        if(beanDefinition->isSingleton()) {         //!< 如果不是单例对象则不需要获取，因为非单例对象无法预存
            getBeanToVariant(beanName, thread);                      //!< 获取一次bean，让bean预加载
        }
    }
}
