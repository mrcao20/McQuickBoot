#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <McBoot/Controller/impl/McResult.h>
#include <McBoot/McQuickBootSimple.h>
#include <McBoot/Utils/Callback/Impl/McCppAsyncCallback.h>
#include <McBoot/Utils/Callback/Impl/McCppSyncCallback.h>
#include <McIoc/ApplicationContext/IMcApplicationContext.h>

#include <QDebug>
#include <QJsonArray>
#include <QThread>
#include <QTimer>

#include "Param.h"
#include "Test.h"

Q_DECLARE_METATYPE(std::function<void(int)>)

void callbackTest(const QJsonObject &var)
{
    qDebug() << "-----+++++++" << var;
}

void callbackTest2(const ParamPtr &str, int ii)
{
    qDebug() << "callback test:" << str << str->aaa << ii;
}

void callbackTest3(int a)
{
    qDebug() << "callback test3:" << a;
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QTimer::singleShot(2000, [&app]() { app.quit(); });
    auto metaObj = &Test::staticMetaObject;
    for (int i = 0; i < metaObj->methodCount(); ++i) {
        auto m = metaObj->method(i);
        qDebug() << m.parameterTypes();
    }
    Test t;
    qRegisterMetaType<std::function<void(int)>>("std::function<void(int)>");
    std::function<void(int)> a = [](int b) { qDebug() << "++++++++++++++" << b; };
    QMetaObject::invokeMethod(&t, "aaa", Q_ARG(std::function<void(int)>, a));
    McQuickBootSimple::init();
    $.connect("aaa", SIGNAL(signal_sig()), "param", SLOT(slot_slt()));
    ParamPtr p = ParamPtr::create();
    $.connect("aaa", SIGNAL(signal_sig3(int)), p.data(), &Param::callbackTest3);
    $.connect("aaa", SIGNAL(signal_sig3(int)), &app, callbackTest3);
    $.connect("aaa", SIGNAL(signal_sig3(int)), &app, [](int a) {
        qDebug() << "lambda callback test3:" << a;
    });
    p->aaa = 1000;
    $.invoke("aaa.aaa", a).then([]() { qDebug() << "aaa.aaa call finished"; });
    $.invoke("aaa.bbb",
             QVariantList() << "lllllllll" << QJsonObject({{"zzz", "mmmm"}})
                            << QVariant::fromValue(p) << QVariant::fromValue(a))
        .then([]() { qDebug() << "aaa.bbb -----"; });
    $.invoke("aaa.bbb",
             "lllllllll",
             QJsonObject({{"zzz", "mmmm"}}),
             QVariant::fromValue(p),
             QVariant::fromValue(a))
        .then(callbackTest);
    $.invoke("aaa.bbb")
        .then([](const QVariant &var) { qDebug() << ">>>>>>>>>>>>>>>>>>>>>>>>>>>" << var; })
        .error([](const QVariant &var) { qDebug() << "error:" << var; });
    $.invoke("aaa.ccc").then(&t, &Test::callbackTest);
    auto res = $.syncInvoke("aaa.ccc");
    auto ccc = [](const QVariant &str, int ii) { qDebug() << "callback test0:" << str << ii; };
    $.invoke("aaa.vvv", McCppSyncCallback::build(callbackTest2)).then([](const QVariant &var) {
        qDebug() << "vvvvvvvvvvvvvv" << var;
    });
    {
        Test t2;
        $.invoke("aaa.aaa", a).then([]() { qDebug() << "aaa.aaa call finished2"; }).attach(&t2);
    }
    $.invoke("aaa.ddd")
        .then([]() { qDebug() << "ddd finished"; })
        .progress([](int cur, int tol) { qDebug() << "ddd return:" << cur << tol; })
        .cancel();
    {
        McCancel c;
        McProgress p;
        $.invoke("aaa.ddd2", c, p).then([]() { qDebug() << "ddd2 finished"; });
        c.cancel();
        p.callback([](int cur, int tot) { qDebug() << "ddd2>>>>>>>" << cur << tot; });
    }
    {
        ParamPtr p = ParamPtr::create();
        $.invoke("aaa.ddd3", p, 3)
            .then([]() { qDebug() << "ddd3 finished"; })
            .progress([](int cur, int tol) { qDebug() << "ddd3 return:" << cur << tol; })
            .cancel();
    }
    qDebug() << res;
    qDebug() << res.value<ParamPtr>()->aaa;
    QHash<int, QString> hhh;
    hhh.insert(-10, "-1");
    hhh.insert(0, "0");
    hhh.insert(10, "1");
    qDebug() << hhh << hhh.keys();
    qDebug() << "1111111111111_>>>>>>"
             << McQuickBootSimple::instance()->getApplicationContext()->getBean<ParamPtr>("param");
    qDebug() << "main thread:" << QThread::currentThread();
    return app.exec();
}
