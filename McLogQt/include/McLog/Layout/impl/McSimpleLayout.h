#pragma once

#include "McPatternLayout.h"

class MCLOGQT_EXPORT McSimpleLayout : public McPatternLayout
{
    Q_OBJECT
    MC_DECL_INIT(McSimpleLayout)
    MC_TYPELIST(McPatternLayout)
public:
    Q_INVOKABLE McSimpleLayout(){}
    
    void finished() noexcept override;
};

MC_DECL_METATYPE(McSimpleLayout)
