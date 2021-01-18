#pragma once

#include "McAbstractNormalBeanFactory.h"

MC_FORWARD_DECL_PRIVATE_DATA(McSharedBeanFactory);

class MCIOC_EXPORT McSharedBeanFactory : public McAbstractNormalBeanFactory
{
    Q_OBJECT
public:
    McSharedBeanFactory(QObject *parent = nullptr) noexcept;
    ~McSharedBeanFactory() override;

    bool canRegister(IMcBeanDefinitionConstPtrRef beanDefinition) noexcept override;
    bool canResolve(McBeanReferenceConstPtrRef beanRef) noexcept override;

protected:
    QVariant convertToQVariant(QObject *obj) noexcept override;
    QVariant convertToQVariant(void *gadget, const QMetaObject *metaObj) noexcept override;

private:
    MC_DECL_PRIVATE(McSharedBeanFactory)
};

MC_DECL_POINTER(McSharedBeanFactory)
