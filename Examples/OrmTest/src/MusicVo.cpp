#include "MusicVo.h"

MC_STATIC()
MC_REGISTER_BEAN_FACTORY(MusicVo)
MC_REGISTER_BEAN_FACTORY(Album)
MC_STATIC_END

MC_DECL_PRIVATE_DATA(MusicVo)
MC_DECL_PRIVATE_DATA_END

MusicVo::MusicVo() noexcept
{
    MC_NEW_PRIVATE_DATA(MusicVo);
}

MusicVo::~MusicVo()
{
}
