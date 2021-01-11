#include "McOrm/DaoLoader/impl/McDaoLoader.h"

#include <QDebug>

#include <McIoc/ApplicationContext/impl/McAnnotationApplicationContext.h>

#include "McOrm/Parser/impl/McSelectParser.h"

MC_INIT(McDaoLoader)
MC_REGISTER_BEAN_FACTORY(McDaoLoader)
MC_INIT_END

MC_DECL_PRIVATE_DATA(McDaoLoader)
QList<IMcParserPtr> parsers;
MC_DECL_PRIVATE_DATA_END

McDaoLoader::McDaoLoader() noexcept
{
    MC_NEW_PRIVATE_DATA(McDaoLoader);
    
    d->parsers.append(McSelectParserPtr::create());
}

McDaoLoader::~McDaoLoader()
{
}

void McDaoLoader::init() noexcept
{
    McAnnotationApplicationContextPtr appCtx = McAnnotationApplicationContextPtr::create();
    init(appCtx);
}

void McDaoLoader::init(IMcApplicationContextConstPtrRef appCtx) noexcept
{
    auto daoNames = Mc::getComponents(appCtx, MC_DAO_TAG);
    QList<QObjectPtr> daos;
    for(auto name : daoNames) {
        auto dao = appCtx->getBean(name);
        if(dao.isNull()) {
            continue;
        }
        daos.append(dao);
    }
    for(auto parser : d->parsers) {
        parser->parse(daos);
    }
}
