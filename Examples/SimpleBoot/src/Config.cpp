#include "Config.h"

#include <QDebug>
#include <QThread>

#include "Param.h"
#include "Test.h"

MC_STATIC()
MC_REGISTER_BEAN_FACTORY(Config)
MC_REGISTER_CONTAINER_CONVERTER(ParamMap)
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

ParamPtr Config::param(QThread *threadTest) const noexcept
{
    qDebug() << "param>>>>" << threadTest;
    return ParamPtr::create();
}

ParamMap Config::paramMap() const noexcept
{
    ParamMap m;
    m.insert("aaa", new Param());
    return m;
}

#include <McBoot/McQuickBootSimple.h>
void Config::finished()
{
    // 和Test中的MC_AUTOWIRED("param")之间必须注释一个
    //    McQuickBootSimple::connect("aaa", SIGNAL(signal_sig2()), this, SLOT(slot_slt2()));
}

void Config::slot_slt2()
{
    qDebug() << "Param::slot_slt2()";
}
