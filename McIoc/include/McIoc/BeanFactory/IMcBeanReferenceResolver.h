#pragma once

#include "../McGlobal.h"

MC_FORWARD_DECL_CLASS(McBeanReference)

class IMcBeanReferenceResolver 
{
public:
    virtual ~IMcBeanReferenceResolver() = default;
    
    /*!
     * \brief resolveBeanReference
     * 
     * 根据beanReference生成实例对象。
     * \param beanRef 被引用对象的信息
     * \return 根据相关信息生成的对象
     */
    virtual QObjectPtr resolveBeanReference(McBeanReferenceConstPtrRef beanRef) noexcept = 0;
    virtual QObject *resolveBeanReferencePointer(McBeanReferenceConstPtrRef beanRef) noexcept = 0;

    virtual bool canResolve(McBeanReferenceConstPtrRef beanRef) noexcept = 0;
    /*!
     * \brief resolveBeanReferenceToQVariant
     * 
     * 根据beanReference生成包含实例对象的QVariant。
     * \param beanRef 被引用对象的信息
     * \return 包含根据相关信息生成的对象的QVariant。
     */
    virtual QVariant resolveBeanReferenceToQVariant(McBeanReferenceConstPtrRef beanRef) noexcept = 0;
};

MC_DECL_POINTER(IMcBeanReferenceResolver)
