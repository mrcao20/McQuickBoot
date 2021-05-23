#include "McIoc/BeanFactory/impl/McSharedBeanFactory.h"

#include <QDebug>

#include "McIoc/BeanDefinition/IMcBeanDefinition.h"
#include "McIoc/BeanFactory/impl/McBeanReference.h"
#include "McIoc/Destroyer/IMcDestroyer.h"
#include "McIoc/Thread/IMcDeleteThreadWhenQuit.h"

MC_DECL_PRIVATE_DATA(McSharedBeanFactory)
MC_DECL_PRIVATE_DATA_END

McSharedBeanFactory::McSharedBeanFactory(QObject *parent) noexcept
    : McAbstractNormalBeanFactory(parent)
{
    MC_NEW_PRIVATE_DATA(McSharedBeanFactory);
}

McSharedBeanFactory::~McSharedBeanFactory()
{
}

bool McSharedBeanFactory::canRegister(IMcBeanDefinitionConstPtrRef beanDefinition) const noexcept
{
    return !beanDefinition->isPointer();
}

bool McSharedBeanFactory::canResolve(McBeanReferenceConstPtrRef beanRef) noexcept
{
    if (!beanRef) {
        qCritical() << "beanReference not exists";
        return false;
    }
    auto pluginPath = beanRef->getPluginPath();
    if (!pluginPath.isEmpty()) {
        return !beanRef->isPointer();
    } else {
        auto defs = getBeanDefinitions();
        if (!defs.contains(beanRef->getName())) {
            return false;
        }
        return canRegister(defs.value(beanRef->getName()));
    }
}

QVariant McSharedBeanFactory::convertToQVariant(QObject *obj) noexcept
{
    QVariant var;
    QObjectPtr bean(obj, Mc::McCustomDeleter());
    var.setValue(bean);
    QString typeName = QString("%1Ptr").arg(bean->metaObject()->className());
    if (!var.convert(QMetaType::type(typeName.toLocal8Bit()))) {
        qCritical() << QString("failed convert QObjectPtr to '%1'").arg(typeName);
        return QVariant();
    }
    auto destoryer = var.value<IMcDeleteThreadWhenQuitPtr>();
    if (destoryer) {
        destoryer->deleteWhenQuit();
    }
    //    auto customDeleter = var.value<IMcDestroyerPtr>();
    //    if (!customDeleter.isNull()) {
    //        //! 这里如果传递共享指针，那么该对象将永远不会析构
    //        bean->setProperty(Mc::Constant::Property::customDeleter,
    //                          QVariant::fromValue(customDeleter.data()));
    //    }
    return var;
}

QVariant McSharedBeanFactory::convertToQVariant(void *gadget, const QMetaObject *metaObj) noexcept
{
    QByteArray clazzName = metaObj->className();
    clazzName.append('*');
    QVariant var(QMetaType::type(clazzName), &gadget);
    QByteArray typeName = metaObj->className();
    typeName.append("Ptr");
    if (!var.convert(QMetaType::type(typeName))) {
        qCritical() << QString("failed convert VoidPtr to '%1'").arg(typeName.data());
        return QVariant();
    }
    return var;
}
