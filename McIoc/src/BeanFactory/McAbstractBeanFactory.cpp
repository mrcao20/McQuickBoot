#include "McIoc/BeanFactory/impl/McAbstractBeanFactory.h"

#include <QMutex>
#include <QDebug>

#include "McIoc/BeanDefinition/impl/McRootBeanDefinition.h"
#include "McIoc/BeanFactory/impl/McBeanReference.h"

MC_DECL_PRIVATE_DATA(McAbstractBeanFactory)
QHash<QString, IMcBeanDefinitionPtr> hash;
QMutex mtx{ QMutex::Recursive };
QThread *targetThread{nullptr};     //!< 生成对象的目标线程
MC_DECL_PRIVATE_DATA_END

McAbstractBeanFactory::McAbstractBeanFactory(QObject *parent)
    : QObject(parent)
{
    MC_NEW_PRIVATE_DATA(McAbstractBeanFactory)
}

McAbstractBeanFactory::~McAbstractBeanFactory()
{}

QObjectPtr McAbstractBeanFactory::getBean(const QString &name, QThread *thread) noexcept
{
    auto var = getBeanToVariant(name, thread);
    if(!var.isValid())
        return QObjectPtr();
    return var.value<QObjectPtr>();
}

QObject *McAbstractBeanFactory::getBeanPointer(const QString &name, QThread *thread) noexcept
{
    auto var = getBeanToVariant(name, thread);
    if (!var.isValid())
        return nullptr;
    return var.value<QObject *>();
}

QVariant McAbstractBeanFactory::getBeanToVariant(const QString &name, QThread *thread)  noexcept 
{
    QMutexLocker locker(&d->mtx);
    d->targetThread = thread;
    auto beanDefinition = d->hash.value(name);
    if (beanDefinition == nullptr) {
        qCritical() << "No bean named " << name << " is defined";
        return QVariant();
    }
    auto beanVar = beanDefinition->getBean();
    if (!beanVar.isValid()) {    //!< 如果bean不存在
        beanVar = doCreate(beanDefinition, d->targetThread);    //!< 创建
        if (!beanVar.isValid()) {
            qWarning() << QString("failed to create bean '%1'").arg(name);
            return QVariant();
        }
        if(beanDefinition->isSingleton())
            beanDefinition->setBean(beanVar);        //!< 如果为单例时，则放进beanDefinition，以达到复用。
    }
    return beanVar;
}

bool McAbstractBeanFactory::containsBean(const QString &name) const noexcept
{
    QMutexLocker locker(&d->mtx);
    return d->hash.contains(name);
}

bool McAbstractBeanFactory::isSingleton(const QString &name) noexcept 
{
    QMutexLocker locker(&d->mtx);
    if(!d->hash.contains(name)) {
        return false;
    }
    auto def = d->hash.value(name);
    return def->isSingleton();
}

bool McAbstractBeanFactory::registerBeanDefinition(
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

IMcBeanDefinitionPtr McAbstractBeanFactory::unregisterBeanDefinition(const QString &name) noexcept
{
    QMutexLocker locker(&d->mtx);
    if(!d->hash.contains(name)) {
        return IMcBeanDefinitionPtr();
    }
    return d->hash.take(name);
}

bool McAbstractBeanFactory::isContained(const QString &name) const noexcept
{
    return containsBean(name);
}

bool McAbstractBeanFactory::canRegister(IMcBeanDefinitionConstPtrRef beanDefinition) const noexcept
{
    Q_UNUSED(beanDefinition)
    return true;
}

QHash<QString, IMcBeanDefinitionPtr> McAbstractBeanFactory::getBeanDefinitions() const noexcept
{
    QMutexLocker locker(&d->mtx);
    return d->hash;
}

QObjectPtr McAbstractBeanFactory::resolveBeanReference(McBeanReferenceConstPtrRef beanRef) noexcept 
{
    auto var = resolveBeanReferenceToQVariant(beanRef);
    if(!var.isValid())
        return QObjectPtr();
    return var.value<QObjectPtr>();
}

QObject *McAbstractBeanFactory::resolveBeanReferencePointer(
    McBeanReferenceConstPtrRef beanRef) noexcept
{
    auto var = resolveBeanReferenceToQVariant(beanRef);
    if (!var.isValid())
        return nullptr;
    return var.value<QObject *>();
}

QVariant McAbstractBeanFactory::resolveBeanReferenceToQVariant(McBeanReferenceConstPtrRef beanRef) noexcept 
{
    if (!beanRef) {
        qCritical() << "beanReference not exists";
        return QVariant();
    }
    auto pluginPath = beanRef->getPluginPath();
    if(!pluginPath.isEmpty()) {
        McRootBeanDefinitionPtr def = McRootBeanDefinitionPtr::create();
        def->setPluginPath(pluginPath);
        auto beanVar = doCreate(def, d->targetThread);
        if (!beanVar.isValid()) {
            qWarning() << QString("failed to create bean of plugin '%1'").arg(pluginPath);
            return QVariant();
        }
        return beanVar;
    }else{
        //! 调用getBeanToVariant方法，根据bean引用的名称获取实例
        return getBeanToVariant(beanRef->getName(), d->targetThread);
    }
}
