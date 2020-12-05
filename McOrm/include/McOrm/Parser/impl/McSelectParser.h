#pragma once

#include "McAbstractParser.h"

MC_FORWARD_DECL_PRIVATE_DATA(McSelectParser);

class McSelectParser : public McAbstractParser
{
    Q_OBJECT
    MC_DEFINE_TYPELIST(MC_DECL_TYPELIST(McAbstractParser))
    MC_DECL_INIT(McSelectParser)
public:
    Q_INVOKABLE McSelectParser() noexcept;
    ~McSelectParser() override;
    
protected:
    IMcSqlSlot *createSqlSlot(const char *tag) noexcept override;
    
private:
    MC_DECL_PRIVATE(McSelectParser)
};

MC_DECL_METATYPE(McSelectParser)
