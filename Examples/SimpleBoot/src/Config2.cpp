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

//MC_SIMPLE_DEFINE_QUICKBOOT_WORK_THREAD(Config2)
//MC_CUSTOM_DEFINE_QUICKBOOT_WORK_THREAD(Config2, { return new QThread(); })

QThread *threadT()
{
    auto t = new CThread();
    QObject::connect(t, &QThread::started, []() { qDebug() << "started........"; });
    QObject::connect(t, &QThread::finished, []() { qDebug() << "finished........"; });
    return t;
}
MC_CUSTOM_FUNC_DEFINE_QUICKBOOT_WORK_THREAD(Config2, threadT)
