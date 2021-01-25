#include "Config2.h"

#include <QDebug>
#include <QThread>

MC_STATIC()
MC_REGISTER_BEAN_FACTORY(Config2)
MC_STATIC_END

MC_DECL_PRIVATE_DATA(Config2)
MC_DECL_PRIVATE_DATA_END

Config2::Config2(QObject *parent) noexcept : QObject(parent)
{
    MC_NEW_PRIVATE_DATA(Config2);
}

Config2::~Config2() {}

QThread *Config2::threadTest() const noexcept
{
    return new QThread();
}
