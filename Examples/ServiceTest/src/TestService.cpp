#include "TestService.h"

#include <QDebug>
#include <QThread>

MC_STATIC()
MC_REGISTER_BEAN_FACTORY(TestService)
MC_STATIC_END

MC_DECL_PRIVATE_DATA(TestService)
MC_DECL_PRIVATE_DATA_END

TestService::TestService(QObject *parent) noexcept
    : QObject(parent)
{
    MC_NEW_PRIVATE_DATA(TestService);

    qDebug() << "TestService" << QThread::currentThread();
}

TestService::~TestService()
{
}

void TestService::timeout() noexcept
{
    qDebug() << "timeout" << QThread::currentThread();
}

int TestService::interval() noexcept
{
    return 10000;
}

void TestService::moveFinished() noexcept
{
    qDebug() << "moveFinished" << QThread::currentThread();
}
