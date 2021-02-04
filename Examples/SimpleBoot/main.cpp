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

void callbackTest2(const QVariant &str, QVariant ii)
{
    qDebug() << "callback test:" << str << ii;
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
    p->aaa = 1000;
    $.invoke("aaa.bbb",
             QVariantList() << "lllllllll" << QJsonObject({{"zzz", "mmmm"}})
                            << QVariant::fromValue(p) << QVariant::fromValue(a))
        .then([](const QJsonObject &var) { qDebug() << "-----" << var; });
    $.invoke("aaa.bbb",
             "lllllllll",
             QJsonObject({{"zzz", "mmmm"}}),
             QVariant::fromValue(p),
             QVariant::fromValue(a))
        .then(callbackTest);
    $.invoke("aaa.bbb").then(
        [](const QVariant &var) { qDebug() << ">>>>>>>>>>>>>>>>>>>>>>>>>>>" << var; });
    $.invoke("aaa.ccc").then(&t, &Test::callbackTest);
    auto res = $.syncInvoke("aaa.ccc");
    auto ccc = [](const QVariant &str, int ii) { qDebug() << "callback test0:" << str << ii; };
    $.invoke("aaa.vvv", McCppAsyncCallback::build(callbackTest2)).then([](const QVariant &var) {
        qDebug() << "vvvvvvvvvvvvvv" << var;
    });
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
