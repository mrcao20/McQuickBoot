#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <McBoot/Controller/impl/McResult.h>
#include <McBoot/McQuickBootSimple.h>
#include <McIoc/ApplicationContext/IMcApplicationContext.h>

#include <QDebug>
#include <QJsonArray>
#include <QTimer>

#include "Param.h"
#include "Test.h"

Q_DECLARE_METATYPE(std::function<void(int)>)

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
    McQuickBootSimple::connect("aaa", SIGNAL(signal_sig()), "param", SLOT(slot_slt()));
    ParamPtr p = ParamPtr::create();
    p->aaa = 1000;
    $.invoke("aaa.bbb",
             QVariantList() << "lllllllll" << QJsonObject({{"zzz", "mmmm"}})
                            << QVariant::fromValue(p) << QVariant::fromValue(a))
        .then([](const QVariant &var) { qDebug() << "-----" << var; });
    $.invoke("aaa.bbb").then(
        [](const QVariant &var) { qDebug() << ">>>>>>>>>>>>>>>>>>>>>>>>>>>" << var; });
    $.invoke("aaa.ccc").then([](const QVariant &var) {
        qDebug() << var;
        qDebug() << var.value<ParamPtr>()->aaa;
    });
    auto res = $.syncInvoke("aaa.ccc");
    qDebug() << res;
    qDebug() << res.value<ParamPtr>()->aaa;
    QHash<int, QString> hhh;
    hhh.insert(-10, "-1");
    hhh.insert(0, "0");
    hhh.insert(10, "1");
    qDebug() << hhh << hhh.keys();
    qDebug() << "1111111111111_>>>>>>"
             << McQuickBootSimple::instance()->getApplicationContext()->getBean<ParamPtr>("param");
    return app.exec();
}
