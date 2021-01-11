#pragma once

#include "../IMcParser.h"

class IMcSqlSlot;

class McAbstractParser : public QObject, public IMcParser
{
    Q_OBJECT
    MC_TYPELIST(IMcParser)
    MC_DECL_INIT(McAbstractParser);
public:
    ~McAbstractParser() override = default;
    
    void parse(const QList<QObjectPtr> &beans) noexcept override;
    
protected:
    virtual IMcSqlSlot *createSqlSlot(const char *tag) noexcept = 0;
};

MC_DECL_METATYPE(McAbstractParser)
