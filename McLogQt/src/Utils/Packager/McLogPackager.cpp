#include "McLog/Utils/Packager/impl/McLogPackager.h"

MC_STATIC()
MC_REGISTER_BEAN_FACTORY(McLogPackager)
MC_STATIC_END

MC_DECL_PRIVATE_DATA(McLogPackager)
MC_DECL_PRIVATE_DATA_END

McLogPackager::McLogPackager() noexcept
{
    MC_NEW_PRIVATE_DATA(McLogPackager);
}

McLogPackager::~McLogPackager()
{
}
