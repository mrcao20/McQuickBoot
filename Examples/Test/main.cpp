#include <QtCore/qglobal.h>
#if QT_VERSION >= 0x050000
#include <QtGui/QGuiApplication>
#include <QtQml/QQmlApplicationEngine>
#else
#endif

#include <QDebug>

#include <corecrt_startup.h>

#include "McIoc/ApplicationContext/impl/McLocalPathApplicationContext.h"
#include "McIoc/ApplicationContext/impl/McAnnotationApplicationContext.h"
#include "McIoc/ApplicationContext/impl/McYamlSettingApplicationContext.h"
#include "IocTest.h"
#include "InvokeTest.h"
#include <Object.h>
#include <McBoot/McQuickBoot.h>
#include "ThreadTest.h"
#include <McBoot/Utils/McJsonUtils.h>

class A
{
public:
    A() { qDebug() << "A"; }
};

static A a;
static int ttt = []() {
    qDebug() << "ttt";
    return 0;
}();

Q_GLOBAL_STATIC(int, aaa)

#ifdef Q_CC_GNU
static void  __attribute__((constructor)) before(void) 
{
    qDebug() << "before";
    qDebug() << Q_QGS_aaa::guard;
    Q_QGS_aaa::guard = 3;
    qDebug() << Q_QGS_aaa::guard;
}

static void  __attribute__((destructor)) after(void) 
{
    qDebug() << "after";
}
#elif Q_CC_MSVC
#define SECNAME ".CRT$XCY"
#pragma section(SECNAME, long, read)
int foo()
{
    qDebug() << "before";
    qDebug() << Q_QGS_aaa::guard;
    Q_QGS_aaa::guard = 2;
    qDebug() << Q_QGS_aaa::guard;
    return 0;
}
__declspec(allocate(SECNAME)) _PIFV dummy[] = {foo};
#endif

#define BB(par) MC_FIRST_TYPE_NAME((par))

int main(int argc, char *argv[])
{
    qDebug() << Q_QGS_aaa::guard;
//    qDebug() << ">>>>>>>>>>:::::" << BB(MC_TYPELIST(IocTest));
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
//    auto mo = QMetaType::metaObjectForType(QMetaType::type("IocTest"));
//    auto me = mo->enumerator(mo->indexOfEnumerator("EEE"));
//    qDebug() << me.enumName();
//    qDebug() << me.keysToValue("A | B");
//    return McQuickBoot::singleRun(argc, argv);
#if QT_VERSION >= QT_VERSION_CHECK(5, 5, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    
    QGuiApplication app(argc, argv);
    
    auto mo = &IocTest::staticMetaObject;
    auto pro = mo->property(mo->indexOfProperty("innerBean"));
    qDebug() << "bbbbbbbbbbb>" << pro.userType() << pro.typeName()
             << QMetaType::type("QObject");
    
    ThreadTest *t = new ThreadTest();
    t->start();
    IMcApplicationContextPtr appCon;
    //! XML注入方式
//    appCon = McLocalPathApplicationContextPtr::create(
//                QStringList() << ":/myspring.xml" << ":/xmltest2.xml");
    appCon = McYamlSettingApplicationContextPtr::create(
                ":/ioc.yml");
    
    appCon->refresh(t);
    auto test = appCon->getBean<IocTestPtr>("test");
    qDebug() << test->m_interface << test->m_str << test->m_interfaces << test->m_interfaces.at(2).dynamicCast<Object>()->m_text
             << test->m_strMap << test->m_iMap << test->m_innerBean << test->m_innerBean.objectCast<Object>()->m_text
             << test->m_eee << test->m_align;
    test->m_interface->say();
    test->m_interface.dynamicCast<Object>()->signal();
    test.dynamicCast<QObject>()->property("inter").value<ObjectPtr>()->signal2();
    qDebug() << test.dynamicCast<QObject>()->property("interStr");
    
//    auto test2 = appCon->getBean<IocTestPtr>("test2");
//    qDebug() << test2->m_innerBean.objectCast<Object>()->m_text;

    //!< end
    
    //! 声明式注入方式
    t = new ThreadTest();
    t->start();
    appCon = McAnnotationApplicationContextPtr::create();
    appCon->refresh();
    QVariant var = appCon->getBeanToVariant("test", t);
    test = var.value<IocTestPtr>();
    qDebug() << test << test->m_interface << appCon->isSingleton("test");
    test->m_interface->say();
    test->m_interface.dynamicCast<Object>()->signal();
    //!< end
    
    
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
