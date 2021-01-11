#pragma once

#include "McPatternLayout.h"

class MCLOGQT_EXPORT McTTCCLayout : public McPatternLayout 
{
    Q_OBJECT
    MC_DECL_INIT(McTTCCLayout)
    MC_TYPELIST(McPatternLayout)
public:
    Q_INVOKABLE McTTCCLayout(){}
    
    void finished() noexcept override;
};

MC_DECL_METATYPE(McTTCCLayout)
