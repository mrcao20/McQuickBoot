#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <McBoot/McQuickBootSimple.h>
#include <McBoot/Controller/impl/McResult.h>

#include <QTimer>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QTimer::singleShot(2000, [&app](){
        app.quit();
    });
    McQuickBootSimple::init();
    $.invoke("aaa.bbb").then([](const QVariant &var) { qDebug() << "-----" << var; });
    QHash<int, QString> hhh;
    hhh.insert(-10, "-1");
    hhh.insert(0, "0");
    hhh.insert(10, "1");
    qDebug() << hhh << hhh.keys();
    return app.exec();
}
