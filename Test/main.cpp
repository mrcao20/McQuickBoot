#include <QtCore/qglobal.h>
#if QT_VERSION >= 0x050000
#include <QtGui/QGuiApplication>
#include <QtQml/QQmlApplicationEngine>
#else
#endif

#include <QDebug>

#include "ApplicationContext/impl/McLocalPathApplicationContext.h"
#include "ApplicationContext/impl/McAnnotationApplicationContext.h"
#include "IocTest.h"
#include <Object.h>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    
    QGuiApplication app(argc, argv);
    
    
    IMcApplicationContextPtr appCon = McLocalPathApplicationContextPtr::create(
                QStringList() << ":/myspring.xml" << ":/xmltest2.xml");
    appCon->refresh();
    auto var = appCon->getBeanToVariant("test");
    auto test = var.value<IocTestPtr>();
    qDebug() << test->m_interface << test->m_str << test->m_interfaces
             << test->m_strMap << test->m_iMap;
    test->m_interface->say();
    test->m_interface.dynamicCast<Object>()->signal();
    
    
    appCon = McAnnotationApplicationContextPtr::create();
    appCon->refresh();
    var = appCon->getBeanToVariant("test");
    test = var.value<IocTestPtr>();
    qDebug() << test << test->m_interface << appCon->isSingleton("test");
    test->m_interface->say();
    test->m_interface.dynamicCast<Object>()->signal();
    
    
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
