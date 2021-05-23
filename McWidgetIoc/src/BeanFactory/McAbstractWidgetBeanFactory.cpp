#include "McWidgetIoc/BeanFactory/Impl/McAbstractWidgetBeanFactory.h"

#include <QDebug>
#include <QMutex>
#include <QWidget>

#include <McIoc/BeanDefinition/IMcBeanDefinition.h>

MC_DECL_PRIVATE_DATA(McAbstractWidgetBeanFactory)
QHash<QString, IMcBeanDefinitionPtr> hash;
QMutex mtx{QMutex::Recursive};
MC_DECL_PRIVATE_DATA_END

McAbstractWidgetBeanFactory::McAbstractWidgetBeanFactory(QObject *parent) : QObject(parent)
{
    MC_NEW_PRIVATE_DATA(McAbstractWidgetBeanFactory);
}

McAbstractWidgetBeanFactory::~McAbstractWidgetBeanFactory() {}

QWidget *McAbstractWidgetBeanFactory::getBean(const QString &name, QWidget *parent) const noexcept
{
    IMcBeanDefinitionPtr beanDefinition;
    {
        QMutexLocker locker(&d->mtx);
        beanDefinition = d->hash.value(name);
        if (beanDefinition.isNull()) {
            qCCritical(mcWidgetIoc()) << "No bean named " << name << " is defined";
            return nullptr;
        }
    }
    auto beanVar = beanDefinition->getBean();
    auto bean = beanVar.value<QWidget *>();
    if (bean == nullptr) {               //!< 如果bean不存在
        bean = doCreate(beanDefinition); //!< 创建
        if (bean == nullptr) {
            qCCritical(mcWidgetIoc()) << QString("failed to create bean '%1'").arg(name);
            return nullptr;
        }
        bean->setParent(parent);
        if (beanDefinition->isSingleton()) {
            beanDefinition->setBean(QVariant::fromValue(bean));
        }
    }
    return bean;
}

bool McAbstractWidgetBeanFactory::containsBean(const QString &name) const noexcept
{
    QMutexLocker locker(&d->mtx);
    return d->hash.contains(name);
}

bool McAbstractWidgetBeanFactory::registerBeanDefinition(
    const QString &name, IMcBeanDefinitionConstPtrRef beanDefinition) noexcept
{
    if (!canRegister(beanDefinition)) {
        return false;
    }
    QMutexLocker locker(&d->mtx);
    //! 如果存在则替换
    d->hash.insert(name, beanDefinition);
    return true;
}

IMcBeanDefinitionPtr McAbstractWidgetBeanFactory::unregisterBeanDefinition(
    const QString &name) noexcept
{
    QMutexLocker locker(&d->mtx);
    if (!d->hash.contains(name)) {
        return IMcBeanDefinitionPtr();
    }
    return d->hash.take(name);
}

bool McAbstractWidgetBeanFactory::isContained(const QString &name) const noexcept
{
    return containsBean(name);
}

bool McAbstractWidgetBeanFactory::canRegister(IMcBeanDefinitionConstPtrRef beanDefinition) const
    noexcept
{
    Q_UNUSED(beanDefinition)
    return true;
}

QHash<QString, IMcBeanDefinitionPtr> McAbstractWidgetBeanFactory::getBeanDefinitions() const
    noexcept
{
    QMutexLocker locker(&d->mtx);
    return d->hash;
}
