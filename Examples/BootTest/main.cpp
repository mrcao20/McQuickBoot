#include <McBoot/McQuickBoot.h>
#include <QDebug>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QTemporaryFile>

int main(int argc, char *argv[])
{
    //    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    //    QGuiApplication app(argc, argv);

    //    QQmlApplicationEngine engine;
    //    engine.load(QUrl(QLatin1String("qrc:/main.qml")));

    //    return app.exec();
    McQuickBoot::addConfigPath(R"(E:\Code\QtCreator\application.yml)");
    return McQuickBoot::run(argc, argv, QLatin1String("qrc:/main.qml"));
}
