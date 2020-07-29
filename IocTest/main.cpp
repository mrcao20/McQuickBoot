#include <QCoreApplication>
#include <QThread>
#include <QDebug>

#include <McIoc/ApplicationContext/impl/McLocalPathApplicationContext.h>
#include <McIoc/ApplicationContext/impl/McAnnotationApplicationContext.h>

#include "C.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
//    auto appContext = McLocalPathApplicationContextPtr::create(R"(E:\Code\QtCreator\McIocBoot\IocTest\myspring.xml)");
    auto appContext = McAnnotationApplicationContextPtr::create();
    QThread *t = new QThread(&a);
    t->start();
    qDebug() << "t:" << t;
    auto ia = appContext->getBean<IA>("c", t);
    ia->a();
    return a.exec();
}
