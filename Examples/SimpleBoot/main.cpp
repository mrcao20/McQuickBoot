#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <McBoot/McQuickBootSimple.h>
#include <McBoot/Controller/impl/McResult.h>

#include <QTimer>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QTimer::singleShot(2000, [&app](){
        app.quit();
    });
    McQuickBootSimple::init();
    $->invoke("aaa.bbb")->then([](const QVariant &var){
        qDebug() << "-----" << var;
    });
    return app.exec();
}
