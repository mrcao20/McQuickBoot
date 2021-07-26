#include "Test.h"

#include <QDebug>
#include <QThread>

MC_STATIC()
MC_REGISTER_BEAN_FACTORY(Test)
MC_REGISTER_MAP_CONVERTER(IIIMap)
MC_REGISTER_LIST_CONVERTER(QList<IIIPtr>)
MC_BOOT_REGISTER_REQUEST(CustomRequestType)
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
    qDebug() << "bbb" << a << obj << p->aaa << param << paramMap << threadTest << res;
    qDebug() << "cur thread:" << QThread::currentThread() << "obj thread:" << thread();
    return QJsonObject({{"ccc", "bbb"}});
}

void Test::aaa(const std::function<void(int)> &func)
{
    func(200);
}

QVariant Test::vvv(const IMcCallbackPtr &callback)
{
    auto p = ParamPtr::create();
    p->aaa = 1111111;
    callback->call(p, 111);
    return QVariant(1);
}

ParamPtr Test::ccc()
{
    auto p = ParamPtr::create();
    p->aaa = 1111111;
    return p;
}

void Test::ddd(const McRequest &req)
{
    QThread::sleep(1);
    auto cancel = req.cancel();
    auto progress = req.progress();
    qDebug() << "dddddddddd>>>>>>" << req.check<int>() << req.at<int>(0) << req.count()
             << cancel.isCanceled();
    progress.setCurrent(10);
}

void Test::ddd2(McCancel c, McProgress p)
{
    qDebug() << "dddddddddd222>>>>>>" << c.isCanceled();
    p.setCurrent(20);
}

void Test::ddd3(const CustomRequestType &req)
{
    QThread::sleep(1);
    auto cancel = req.cancel();
    auto progress = req.progress();
    qDebug() << "dddddddddd33333>>>>>>" << req.check<ParamPtr, ParamPtr>() << req.at<ParamPtr>(0)
             << req.at<int>(1) << req.count() << cancel.isCanceled();
    progress.setCurrent(30);
}
