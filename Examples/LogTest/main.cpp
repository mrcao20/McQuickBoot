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

#include <McIoc/ApplicationContext/IMcApplicationContext.h>

#include "McLog/Appender/IMcRequestableNextFile.h"
#include "McLog/Configurator/McDefaultConfigurator.h"
#include "McLog/Configurator/McINIConfigurator.h"
#include "McLog/Configurator/McXMLConfigurator.h"

#define MC_PRINT_ERR(...) \
    fprintf(stderr, __VA_ARGS__); \
    fflush(stderr);

int main(int argc, char *argv[])
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 5, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    
    QGuiApplication app(argc, argv);
    
    QUrl u("file:///./a");
    qDebug() << "u:" << u.isLocalFile();
    //! 必须开启IOC支持
    auto path = R"(../../Examples/LogTest/logqt.xml)";
#ifdef QT_DEBUG
    auto appCtx = McXMLConfigurator::configure(path, "debug");
#else
    auto appCtx = McXMLConfigurator::configure(path, "release");
#endif
    //    McDefaultConfigurator::configure();
    //    McINIConfigurator::configure(R"(E:/QtCreator/McLogQt/Test/logqt.ini)");
    auto appender = appCtx->getBean<IMcRequestableNextFile>("dailyRollingFile");
    qInfo() << appender;
    appender->forceRequestNextFile();

    qDebug() << "debug";
    qWarning() << "warning";
    qCritical() << "critical";
    qInfo() << "info";
    qInfo() << "中文";
    qInfo() << QStringLiteral("中文2");
    qInfo() << QStringLiteral(u"中文3");
    qDebug(MC_LOGGER("Lidar[0]")) << "Lidar[0]debug";
    qWarning(MC_LOGGER("Lidar[0]")) << "Lidar[0]warning";
    qCritical(MC_LOGGER("Lidar[0]")) << "Lidar[0]critical";
    qInfo(MC_LOGGER("Lidar[0]")) << "Lidar[0]info";
    qInfo(MC_LOGGER("TestLogger")) << "test logger";
    qInfo(MC_LOGGER("TestLogger2")) << "test logger";
    //    QtConcurrent::run([]() {
    //        for (int i = 0; i < 100; i++) {
    //            //            QThread::msleep(100);
    //            qDebug() << i;
    //            if (i == 10) {
    //                //                qFatal("aa");
    //            }
    //        }
    //    });

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
