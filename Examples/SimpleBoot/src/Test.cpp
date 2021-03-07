#include "Test.h"

#include <QDebug>
#include <QThread>

MC_STATIC()
MC_REGISTER_BEAN_FACTORY(Test)
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

QJsonObject Test::bbb(const QString &a,
                      const QJsonObject &obj,
                      const ParamPtr &p,
                      const std::function<void(int)> &func)
{
    emit signal_sig();
    emit signal_sig2();
    emit signal_sig3(100);
    func(300);
    qDebug() << "bbb" << a << obj << p->aaa << param << paramMap << threadTest;
    qDebug() << "cur thread:" << QThread::currentThread() << "obj thread:" << thread();
    return QJsonObject({{"ccc", "bbb"}});
}

void Test::aaa(const std::function<void(int)> &func)
{
    func(200);
}

QVariant Test::vvv(const IMcCallbackPtr &callback)
{
    callback->call("aaa", 111);
    return QVariant(1);
}

ParamPtr Test::ccc()
{
    auto p = ParamPtr::create();
    p->aaa = 1111111;
    return p;
}
