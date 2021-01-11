#include "McOrm/Parser/impl/McSelectParser.h"

#include "McOrm/McOrmGlobal.h"
#include "McOrm/SqlSlot/impl/McSelectSqlSlot.h"

MC_INIT(McSelectParser)
MC_REGISTER_BEAN_FACTORY(McSelectParser)
MC_INIT_END

MC_DECL_PRIVATE_DATA(McSelectParser)
MC_DECL_PRIVATE_DATA_END

McSelectParser::McSelectParser() noexcept
{
    MC_NEW_PRIVATE_DATA(McSelectParser);
}

McSelectParser::~McSelectParser()
{
}

IMcSqlSlot *McSelectParser::createSqlSlot(const char *tag) noexcept
{
    if(qstrcmp(tag, MC_SELECT_TAG) != 0) {
        return nullptr;
    }
    return new McSelectSqlSlot();
}
