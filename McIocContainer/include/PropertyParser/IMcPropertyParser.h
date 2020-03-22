#pragma once

#include "../McGlobal.h"

QT_BEGIN_NAMESPACE
class QDomElement;
QT_END_NAMESPACE

MC_BEGIN_NAMESPACE

class IMcPropertyParser {
public:
    virtual ~IMcPropertyParser() = default;
    
    virtual QVariant parse(const QDomElement &ele) const noexcept = 0;
};

MC_END_NAMESPACE

MC_FORWARD_DECL_CLASS(IMcPropertyParser)
