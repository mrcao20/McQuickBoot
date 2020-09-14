#include <QtCore/qglobal.h>
#if QT_VERSION >= 0x050000
#include <QtGui/QGuiApplication>
#include <QtQml/QQmlApplicationEngine>
#else
#endif
#include <QtDebug>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QtConcurrent>
#include <QSettings>
#include <QUrl>

#include "McLog/McLogManager.h"
#include "McLog/Configurator/McXMLConfigurator.h"
#include "McLog/Configurator/McINIConfigurator.h"

#define MC_PRINT_ERR(...) \
    fprintf(stderr, __VA_ARGS__); \
    fflush(stderr);

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    
    QGuiApplication app(argc, argv);
    
    QUrl u("file:///./a");
    qDebug() << "u:" << u.isLocalFile();
    //! 必须开启IOC支持
    auto path = qApp->applicationDirPath() + "/logqt.xml";
    McXMLConfigurator::configure(path);
//    McINIConfigurator::configure(R"(E:\QtCreator\McLogQt\Test\logqt.ini)");
    McLogManager::installQtMessageHandler();
    
    qDebug() << "debug";
    qWarning() << "warning";
    qCritical() << "critical";
    qInfo() << "info";
    qDebug(MC_LOGGER("Lidar[0]")) << "Lidar[0]debug";
    qWarning(MC_LOGGER("Lidar[0]")) << "Lidar[0]warning";
    qCritical(MC_LOGGER("Lidar[0]")) << "Lidar[0]critical";
    qInfo(MC_LOGGER("Lidar[0]")) << "Lidar[0]info";
    QtConcurrent::run([](){
        for(int i = 0; i < 100; i++) {
            qDebug() << i;
            if(i == 10) {
//                qFatal("aa");
            }
        }
    });
    
    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);
    
    return app.exec();
}
