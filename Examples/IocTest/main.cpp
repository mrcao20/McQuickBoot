#include <QBuffer>
#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QThread>

#include <McIoc/ApplicationContext/impl/McLocalPathApplicationContext.h>
#include <McIoc/ApplicationContext/impl/McAnnotationApplicationContext.h>
#include <McIoc/ApplicationContext/impl/McXmlApplicationContext.h>

#include "C.h"

#include <McIoc/Utils/XmlBuilder/impl/McBean.h>
#include <McIoc/Utils/XmlBuilder/impl/McBeanCollection.h>
#include <McIoc/Utils/XmlBuilder/impl/McConnect.h>
#include <McIoc/Utils/XmlBuilder/impl/McEnum.h>
#include <McIoc/Utils/XmlBuilder/impl/McList.h>
#include <McIoc/Utils/XmlBuilder/impl/McMap.h>
#include <McIoc/Utils/XmlBuilder/impl/McPlaceholder.h>
#include <McIoc/Utils/XmlBuilder/impl/McProperty.h>
#include <McIoc/Utils/XmlBuilder/impl/McRef.h>
#include <McIoc/Utils/XmlBuilder/impl/McValue.h>
#include <QDomDocument>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    
    using namespace McXmlBuilder;
    McBeanCollection bc;
    McBeanPtr b1 = McBeanPtr::create();
    bc.addBean(b1);
    b1->setBeanName("r");
    b1->setClassName("R");
    
    McBeanPtr b = McBeanPtr::create();
    bc.addBean(b);
    b->setBeanName("c");
    b->setClassName("C");
    McPropertyPtr p = McPropertyPtr::create();
    b->addContent(p);
    p->setContent("text", "test c");
    McConnectPtr c = McConnectPtr::create();
    b->addContent(c);
    c->setSender("this");
    c->setSignal("signal_send()");
    c->setReceiver("r");
    c->setSlot("slot_recv()");
    c->setConnectionType("DirectConnection | UniqueConnection");
    
    McEnumPtr en = McEnumPtr::create();
    en->setScope("Qt");
    en->setType("AlignmentFlag");
    en->setValue("AlignLeft");
    McPropertyPtr pp1 = McPropertyPtr::create();
    pp1->setContent("align", en);
    b->addContent(pp1);
    
    McRefPtr ref = McRefPtr::create();
    ref->setBeanName("r");
    McPropertyPtr p3 = McPropertyPtr::create();
    b->addContent(p3);
    p3->setContent("r", ref);
    
    McMapPtr m = McMapPtr::create();
    m->addContent("jack", ref);
    McPropertyPtr p1 = McPropertyPtr::create();
    b->addContent(p1);
    p1->setContent("hrs", m);
    McPlaceholderPtr plh = McPlaceholderPtr::create();
    plh->setPlaceholder("objectName");
    m->addContent(plh, ref);

    McListPtr ll = McListPtr::create();
    ll->addContent("停封");
    McPropertyPtr pp2 = McPropertyPtr::create();
    pp2->setContent("texts", ll);
    b->addContent(pp2);
    
    auto doc = bc.toDomDocument();
    qDebug() << doc.toString(4);
    QFile file(qApp->applicationDirPath() + "/test.xml");
    file.open(QIODevice::WriteOnly);
    QTextStream stream(&file);
    doc.save(stream, 4);

    QSharedPointer<QBuffer> buf = QSharedPointer<QBuffer>::create();
    buf->open(QIODevice::ReadWrite);
    bc.writeToDevice(buf.data());

    auto appContext = McLocalPathApplicationContextPtr::create(
        R"(..\..\Examples\IocTest\myspring.xml)");
    //    auto appContext = McAnnotationApplicationContextPtr::create();
    //    auto appContext = McXmlApplicationContextPtr::create(buf.objectCast<QIODevice>());
    QThread *t = new QThread(&a);
    t->start();
    qDebug() << "t:" << t;
    auto ia = appContext->getBean<IA>("c", t);
    auto gadget = appContext->getBean<GadgetTest>("gadgetTest");
    qDebug() << gadget->aaa << gadget->bbb;
    ia->a();
    return a.exec();
}
