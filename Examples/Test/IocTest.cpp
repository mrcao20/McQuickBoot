#include "IocTest.h"

#include "McIoc/McGlobal.h"

MC_AUTO_INIT(IocTest)
//MC_REGISTER_BEAN_FACTORY(MC_TYPELIST(IocTest))
//Mc::Ioc::connect("test", "interface", "signal()", "this", "slot()");
MC_CONNECT("interface", "signal()", "this", "slot()");
MC_REGISTER_LIST_CONVERTER(QVector<InterfacePtr>)
MC_REGISTER_LIST_CONVERTER(QList<QString>)
MC_REGISTER_MAP_CONVERTER(QStringMap)
MC_REGISTER_MAP_CONVERTER(IHash)
MC_INIT_END

IocTest::IocTest(QObject *parent) : QObject(parent)
{
}
