#include "Test.h"

#include <QDebug>

MC_INIT(Test)
MC_REGISTER_BEAN_FACTORY(MC_TYPELIST(Test))
MC_INIT_END

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
