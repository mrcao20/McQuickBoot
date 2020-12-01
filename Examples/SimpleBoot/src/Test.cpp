#include "Test.h"

#include <QDebug>

MC_STATIC()
MC_REGISTER_BEAN_FACTORY(MC_TYPELIST(Test))
MC_DESTROY()
qDebug() << "destroy..............";
MC_STATIC_END

MC_DECL_PRIVATE_DATA(Test)
MC_DECL_PRIVATE_DATA_END

Test::Test(QObject *parent)
    : QObject(parent)
{
    MC_NEW_PRIVATE_DATA(Test);
}

Test::~Test()
{
}

QJsonObject Test::bbb()
{
    qDebug() << "bbb";
    return QJsonObject({{"ccc", "bbb"}});
}
