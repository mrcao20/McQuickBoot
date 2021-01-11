#include "Param.h"

#include <QDebug>

MC_STATIC()
MC_REGISTER_BEAN_FACTORY(Param)
MC_STATIC_END

MC_DECL_PRIVATE_DATA(Param)
MC_DECL_PRIVATE_DATA_END

Param::Param(QObject *parent) noexcept
    : QObject(parent)
{
    MC_NEW_PRIVATE_DATA(Param);
}

Param::~Param()
{
}

void Param::slot_slt()
{
    qDebug() << "Param::slot_slt()";
}
