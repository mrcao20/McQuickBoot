#include "IocTest.h"

#include "McIoc/ApplicationContext/McContainerGlobal.h"

MC_INIT(IocTest)
MC_REGISTER_COMPONENT(MC_TYPELIST(IocTest))
mcConnect("test", "interface", "signal()", "this", "slot()");
MC_REGISTER_LIST_CONVERTER(QVector<InterfacePtr>)
MC_REGISTER_LIST_CONVERTER(QList<QString>)
MC_REGISTER_MAP_CONVERTER(QStringMap)
MC_REGISTER_MAP_CONVERTER(IHash)
MC_INIT_END

IocTest::IocTest(QObject *parent) : QObject(parent)
{
}
