#include "Object.h"

#include "McIoc/ApplicationContext/McContainerGlobal.h"

MC_INIT(Tmp)
MC_REGISTER_BEAN_FACTORY(Tmp)
MC_INIT_END

MC_INIT(Object)
MC_REGISTER_BEAN_FACTORY(MC_TYPELIST(Object))
MC_REGISTER_LIST_CONVERTER(QList<QList<TmpPtr>>)
MC_REGISTER_LIST_CONVERTER(QList<THash>)
MC_REGISTER_MAP_CONVERTER(THash)
MC_REGISTER_MAP_CONVERTER(TTHash)
MC_INIT_END

Object::Object(QObject *parent) : QObject(parent)
{
}
