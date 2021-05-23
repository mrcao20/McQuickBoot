#include "McIoc/BeanFactory/impl/McPointerBeanFactory.h"

#include <QDebug>

#include "McIoc/BeanDefinition/IMcBeanDefinition.h"
#include "McIoc/BeanFactory/impl/McBeanReference.h"
#include "McIoc/Thread/IMcDeleteThreadWhenQuit.h"

MC_DECL_PRIVATE_DATA(McPointerBeanFactory)
MC_DECL_PRIVATE_DATA_END

McPointerBeanFactory::McPointerBeanFactory(QObject *parent) noexcept
    : McAbstractNormalBeanFactory(parent)
{
    MC_NEW_PRIVATE_DATA(McPointerBeanFactory);
}

McPointerBeanFactory::~McPointerBeanFactory() {}

bool McPointerBeanFactory::canRegister(IMcBeanDefinitionConstPtrRef beanDefinition) const noexcept
{
    return beanDefinition->isPointer();
}

bool McPointerBeanFactory::canResolve(McBeanReferenceConstPtrRef beanRef) noexcept
{
    if (!beanRef) {
        qCritical() << "beanReference not exists";
        return false;
    }
    auto pluginPath = beanRef->getPluginPath();
    if (!pluginPath.isEmpty()) {
        return beanRef->isPointer();
    } else {
        auto defs = getBeanDefinitions();
        if (!defs.contains(beanRef->getName())) {
            return false;
        }
        return canRegister(defs.value(beanRef->getName()));
    }
}

QVariant McPointerBeanFactory::convertToQVariant(QObject *obj) noexcept
{
    QVariant var;
    var.setValue(obj);
    QString typeName = QString("%1*").arg(obj->metaObject()->className());
    if (!var.convert(QMetaType::type(typeName.toLocal8Bit()))) {
        qCritical() << QString("failed convert QObject* to '%1'").arg(typeName);
        return QVariant();
    }
    auto destoryer = var.value<IMcDeleteThreadWhenQuitPtr>();
    if (destoryer) {
        destoryer->deleteWhenQuit();
    }
    return var;
}

QVariant McPointerBeanFactory::convertToQVariant(void *gadget, const QMetaObject *metaObj) noexcept
{
    QByteArray clazzName = metaObj->className();
    clazzName.append('*');
    QVariant var(QMetaType::type(clazzName), &gadget);
    return var;
}
