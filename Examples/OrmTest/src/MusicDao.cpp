#include "MusicDao.h"

MC_STATIC()
MC_REGISTER_BEAN_FACTORY(MusicDao)
MC_REGISTER_CONTAINER_CONVERTER(QList<MusicVoPtr>)
MC_STATIC_END

MC_DECL_PRIVATE_DATA(MusicDao)
MC_DECL_PRIVATE_DATA_END

MusicDao::MusicDao(QObject *parent) noexcept
    : QObject(parent)
{
    MC_NEW_PRIVATE_DATA(MusicDao);
}

MusicDao::~MusicDao()
{
}
