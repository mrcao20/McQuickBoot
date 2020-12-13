#pragma once

#include "../McGlobal.h"

QT_BEGIN_NAMESPACE
class QDomElement;
QT_END_NAMESPACE

class IMcBeanReferenceResolver;

class IMcPropertyConverter 
{
public:
    virtual ~IMcPropertyConverter() = default;
    
    virtual QVariant convert(IMcBeanReferenceResolver *resolver,
                             const QVariant &value) const noexcept = 0;
};

MC_DECL_POINTER(IMcPropertyConverter)
