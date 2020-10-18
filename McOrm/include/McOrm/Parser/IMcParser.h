#pragma once

#include <McIoc/McGlobal.h>

class IMcParser
{
public:
    virtual ~IMcParser() = default;
    
    virtual void parse(const QList<QObjectPtr> &beans) noexcept = 0;
};

MC_DECL_METATYPE(IMcParser)
