#include "HandlerTest.h"

#include <QDebug>

MC_STATIC()
MC_REGISTER_BEAN_FACTORY(HandlerTest)
MC_REGISTER_BEAN_FACTORY(HandlerTest2)
MC_STATIC_END

bool HandlerTest::handler(QVariant &body) noexcept
{
    qDebug() << "handler:" << body;
    return true;
}

bool HandlerTest2::handler(QVariant &body) noexcept
{
    qDebug() << "handler2:" << body;
    return true;
}
