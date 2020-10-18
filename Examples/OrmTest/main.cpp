#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <McIoc/ApplicationContext/impl/McAnnotationApplicationContext.h>
#include <McOrm/DaoLoader/impl/McDaoLoader.h>

#include "TestDao.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    
    McAnnotationApplicationContextPtr appCtx = McAnnotationApplicationContextPtr::create();
    appCtx->getBean<McDaoLoader>("daoLoader")->init(appCtx);
    
    auto testDao = appCtx->getBean<TestDao>("testDao");
    QList<TestVoPtr> lr;
    TestVoPtr r = TestVoPtr::create();
    r->id = 1;
    lr.append(r);
    testDao->selectById(lr);
    qDebug() << lr;
    
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    
    return app.exec();
}
