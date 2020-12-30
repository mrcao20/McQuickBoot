#include "Config.h"

#include <QDebug>

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

ParamPtr Config::param() const noexcept
{
    return ParamPtr::create();
}

#include <McBoot/McQuickBootSimple.h>
void Config::finished()
{
    McQuickBootSimple::connect("aaa", SIGNAL(signal_sig2()), this, SLOT(slot_slt2()));
}

void Config::slot_slt2()
{
    qDebug() << "Param::slot_slt2()";
}
