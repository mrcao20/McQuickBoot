#include "Config.h"

#include "Param.h"
#include "Test.h"

MC_STATIC()
MC_REGISTER_BEAN_FACTORY(MC_TYPELIST(Config))
MC_STATIC_END

MC_DECL_PRIVATE_DATA(Config)
MC_DECL_PRIVATE_DATA_END

Config::Config(QObject *parent) noexcept
    : QObject(parent)
{
    MC_NEW_PRIVATE_DATA(Config);
}

Config::~Config()
{
}

ParamPtr Config::param(const TestPtr &aaa) const noexcept
{
    qDebug() << "Config>>>>>>>>>>>>" << aaa << aaa->ccc();
    return ParamPtr::create();
}
