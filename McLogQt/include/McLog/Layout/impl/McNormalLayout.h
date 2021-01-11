#pragma once

#include "McPatternLayout.h"

class MCLOGQT_EXPORT McNormalLayout : public McPatternLayout 
{
    Q_OBJECT
    MC_DECL_INIT(McNormalLayout)
    MC_TYPELIST(McPatternLayout)
public:
    Q_INVOKABLE McNormalLayout(){}
    
    void finished() noexcept override;
};

MC_DECL_METATYPE(McNormalLayout)
