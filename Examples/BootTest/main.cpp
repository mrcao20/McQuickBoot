#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <McBoot/McIocBoot.h>
#include <QTemporaryFile>
#include <QDebug>

int main(int argc, char *argv[])
{
//    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
//    QGuiApplication app(argc, argv);
    
//    QQmlApplicationEngine engine;
//    engine.load(QUrl(QLatin1String("qrc:/main.qml")));
    
//    return app.exec();
    return McIocBoot::run(argc, argv, QLatin1String("qrc:/main.qml"));
}
