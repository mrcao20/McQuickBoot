#include <QtCore/qglobal.h>
#if QT_VERSION >= 0x050000
#include <QtGui/QGuiApplication>
#include <QtQml/QQmlApplicationEngine>
#else
#endif

#include <QDebug>

#include "McIoc/ApplicationContext/impl/McLocalPathApplicationContext.h"
#include "McIoc/ApplicationContext/impl/McAnnotationApplicationContext.h"
#include "IocTest.h"
#include "InvokeTest.h"
#include <Object.h>
#include <McBoot/McIocBoot.h>
#include "ThreadTest.h"
#include <McBoot/Utils/McJsonUtils.h>

#ifdef Q_CC_GNU
static void  __attribute__((constructor)) before(void) 
{
	qDebug() << "before";
}

static void  __attribute__((destructor)) after(void) 
{
	qDebug() << "after";
}
#elif Q_CC_MSVC
#define SECNAME ".CRT$XCG"
#pragma section(SECNAME,long,read)
int foo()
{
    qDebug() << "before";
    return 0;
}
__declspec(allocate(SECNAME)) _PIFV dummy[] = { foo };
#endif

#define BB(par) MC_FIRST_TYPE_NAME((par))

int main(int argc, char *argv[])
{
    qDebug() << ">>>>>>>>>>:::::" << BB(MC_TYPELIST(IocTest));
//    QList<IocTestPtr> r;
//    McJsonUtils::toJson(r);
//    auto ft = QMetaType::typeFlags(QMetaType::type("QVector<InterfacePtr>"));
//    auto ftt = QMetaType::typeFlags(QMetaType::type("InvokeTestPtr"));
//    qDebug() << ft.testFlag(QMetaType::TypeFlag::PointerToQObject)
//             << ft.testFlag(QMetaType::TypeFlag::SharedPointerToQObject)
//             << ft.testFlag(QMetaType::TypeFlag::TrackingPointerToQObject)
//             << ft.testFlag(QMetaType::TypeFlag::WasDeclaredAsMetaType);
//    qDebug() << ftt.testFlag(QMetaType::TypeFlag::PointerToQObject)
//             << ftt.testFlag(QMetaType::TypeFlag::SharedPointerToQObject)
//             << ftt.testFlag(QMetaType::TypeFlag::TrackingPointerToQObject)
//             << ftt.testFlag(QMetaType::TypeFlag::WasDeclaredAsMetaType);
    return McIocBoot::run(argc, argv);
//    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    
//    QGuiApplication app(argc, argv);
    
//    ThreadTest *t = new ThreadTest();
//    //! XML注入方式
//    IMcApplicationContextPtr appCon = McLocalPathApplicationContextPtr::create(
//                QStringList() << ":/myspring.xml" << ":/xmltest2.xml");
//    appCon->refresh(t);
//    auto test = appCon->getBean<IocTestPtr>("test");
//    qDebug() << test->m_interface << test->m_str << test->m_interfaces
//             << test->m_strMap << test->m_iMap;
//    test->m_interface->say();
//    test->m_interface.dynamicCast<Object>()->signal();
//    test.dynamicCast<QObject>()->property("inter").value<ObjectPtr>()->signal2();
//    qDebug() << test.dynamicCast<QObject>()->property("interStr");
//    t->start();
//    //!< end
    
//    //! 声明式注入方式
//    t = new ThreadTest();
//    t->start();
//    appCon = McAnnotationApplicationContextPtr::create();
//    appCon->refresh();
//    QVariant var = appCon->getBeanToVariant("test", t);
//    test = var.value<IocTestPtr>();
//    qDebug() << test << test->m_interface << appCon->isSingleton("test");
//    test->m_interface->say();
//    test->m_interface.dynamicCast<Object>()->signal();
//    //!< end
    
    
//    QQmlApplicationEngine engine;
//    const QUrl url(QStringLiteral("qrc:/main.qml"));
//    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
//                     &app, [url](QObject *obj, const QUrl &objUrl) {
//        if (!obj && url == objUrl)
//            QCoreApplication::exit(-1);
//    }, Qt::QueuedConnection);
//    engine.load(url);
    
//    return app.exec();
}
