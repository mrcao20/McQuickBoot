#pragma once

#include "McAbstractNormalBeanFactory.h"

MC_FORWARD_DECL_PRIVATE_DATA(McPointerBeanFactory);

class MCIOC_EXPORT McPointerBeanFactory : public McAbstractNormalBeanFactory
{
    Q_OBJECT
public:
    McPointerBeanFactory(QObject *parent = nullptr) noexcept;
    ~McPointerBeanFactory() override;

    bool canRegister(IMcBeanDefinitionConstPtrRef beanDefinition) noexcept override;
    bool canResolve(McBeanReferenceConstPtrRef beanRef) noexcept override;

protected:
    QVariant convertToQVariant(QObject *obj) noexcept override;
    QVariant convertToQVariant(void *gadget, const QMetaObject *metaObj) noexcept override;

private:
    MC_DECL_PRIVATE(McPointerBeanFactory)
};

MC_DECL_POINTER(McPointerBeanFactory)
