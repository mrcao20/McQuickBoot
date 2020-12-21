#include "McIoc/ApplicationContext/impl/McAbstractApplicationContext.h"

#include "McIoc/BeanFactory/IMcBeanFactory.h"
#include "McIoc/BeanDefinition/IMcBeanDefinition.h"

MC_DECL_PRIVATE_DATA(McAbstractApplicationContext)
IMcConfigurableBeanFactoryPtr configurableBeanFactory;
QList<IMcApplicationContextPtr> relatedAppCtx;
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

QObjectPtr McAbstractApplicationContext::getBean(const QString &name, QThread *thread) noexcept
{
    auto var = getBeanToVariant(name, thread);
    if(!var.isValid())
        return QObjectPtr();
    return var.value<QObjectPtr>();
}

QVariant McAbstractApplicationContext::getBeanToVariant(const QString &name,
                                                        QThread *thread) noexcept
{
    QVariant var;
    if(d->configurableBeanFactory->containsBean(name)) {
        var = d->configurableBeanFactory->getBeanToVariant(name, thread);
    } else {
        for(auto appCtx : d->relatedAppCtx) {
            if(appCtx->containsBean(name)) {
                var = appCtx->getBeanToVariant(name, thread);
                break;
            }
        }
    }
    return var;
}

bool McAbstractApplicationContext::containsBean(const QString &name) noexcept
{
    bool isContained = false;
    if(d->configurableBeanFactory->containsBean(name)) {
        isContained = true;
    } else {
        for(auto appCtx : d->relatedAppCtx) {
            if(appCtx->containsBean(name)) {
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
        for(auto appCtx : d->relatedAppCtx) {
            if(appCtx->isSingleton(name)) {
                has = true;
                break;
            }
        }
    }
    return has;
}

void McAbstractApplicationContext::registerBeanDefinition(
    const QString &name, IMcBeanDefinitionConstPtrRef beanDefinition) noexcept
{
    registerBeanDefinitionSelf(name, beanDefinition);
}

IMcBeanDefinitionPtr McAbstractApplicationContext::unregisterBeanDefinition(const QString &name) noexcept
{
    if(d->configurableBeanFactory->containsBean(name)) {
        return d->configurableBeanFactory->unregisterBeanDefinition(name);
    } else {
        for(auto appCtx : d->relatedAppCtx) {
            if(appCtx->containsBean(name)) {
                return appCtx->unregisterBeanDefinition(name);
            }
        }
    }
    return IMcBeanDefinitionPtr();
}

bool McAbstractApplicationContext::isContained(const QString &name) noexcept 
{
    return containsBean(name);
}

QHash<QString, IMcBeanDefinitionPtr> McAbstractApplicationContext::getBeanDefinitions() noexcept 
{
    auto beanDefinitions = d->configurableBeanFactory->getBeanDefinitions();
    for(auto appCtx : d->relatedAppCtx) {
        auto child = appCtx->getBeanDefinitions();
        for(auto key : child.keys()) {
            beanDefinitions.insert(key, child.value(key));
        }
    }
    return beanDefinitions;
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

QVariant McAbstractApplicationContext::getBeanSelf(const QString &name, QThread *thread) noexcept
{
    return d->configurableBeanFactory->getBeanToVariant(name, thread);
}

bool McAbstractApplicationContext::containsBeanSelf(const QString &name) noexcept
{
    return d->configurableBeanFactory->containsBean(name);
}

bool McAbstractApplicationContext::isSingletonSelf(const QString &name) noexcept
{
    return d->configurableBeanFactory->isSingleton(name);
}

void McAbstractApplicationContext::registerBeanDefinitionSelf(
    const QString &name, IMcBeanDefinitionConstPtrRef beanDefinition) noexcept
{
    d->configurableBeanFactory->registerBeanDefinition(name, beanDefinition);
}

IMcBeanDefinitionPtr McAbstractApplicationContext::unregisterBeanDefinitionSelf(const QString &name) noexcept
{
    return d->configurableBeanFactory->unregisterBeanDefinition(name);
}

QHash<QString, IMcBeanDefinitionPtr> McAbstractApplicationContext::getBeanDefinitionsSelf() noexcept
{
    return d->configurableBeanFactory->getBeanDefinitions();
}

void McAbstractApplicationContext::addRelatedApplicationContext(IMcApplicationContextConstPtrRef appCtx) noexcept
{
    if(d->relatedAppCtx.contains(appCtx)) {
        return;
    }
    d->relatedAppCtx.append(appCtx);
}

void McAbstractApplicationContext::removeRelatedApplicationContext(IMcApplicationContextConstPtrRef appCtx) noexcept
{
    if(!d->relatedAppCtx.contains(appCtx)) {
        return;
    }
    d->relatedAppCtx.removeAll(appCtx);
}

QList<IMcApplicationContextPtr> McAbstractApplicationContext::getRelatedApplicationContexts() noexcept
{
    return d->relatedAppCtx;
}
