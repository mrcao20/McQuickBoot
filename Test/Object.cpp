#include "Object.h"

#include "ApplicationContext/McContainerGlobal.h"

MC_INIT(Tmp)
MC_REGISTER_COMPONENT(Tmp)
MC_INIT_END

MC_INIT(Object)
MC_REGISTER_COMPONENT(MC_TYPELIST(Object))
MC_INIT_END

Object::Object(QObject *parent) : QObject(parent)
{
}
