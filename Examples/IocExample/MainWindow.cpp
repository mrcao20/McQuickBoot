#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QFile>

#include <McIoc/ApplicationContext/Impl/McLocalPathApplicationContext.h>
#include <McIoc/Utils/XmlBuilder/Impl/McBeanCollection.h>
#include <McIoc/Utils/XmlBuilder/Impl/McClassBean.h>
#include <McIoc/Utils/XmlBuilder/Impl/McConnect.h>
#include <McIoc/Utils/XmlBuilder/Impl/McConstructorArg.h>
#include <McIoc/Utils/XmlBuilder/Impl/McEnum.h>
#include <McIoc/Utils/XmlBuilder/Impl/McList.h>
#include <McIoc/Utils/XmlBuilder/Impl/McMap.h>
#include <McIoc/Utils/XmlBuilder/Impl/McPlaceholder.h>
#include <McIoc/Utils/XmlBuilder/Impl/McPluginBean.h>
#include <McIoc/Utils/XmlBuilder/Impl/McPluginChecker.h>
#include <McIoc/Utils/XmlBuilder/Impl/McProperty.h>
#include <McIoc/Utils/XmlBuilder/Impl/McRef.h>
#include <McIoc/Utils/XmlBuilder/Impl/McValue.h>

#include "../CoreExample/SimpleInterface.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->pushButton, &QAbstractButton::clicked, this, &MainWindow::slot_buttonClick);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slot_buttonClick()
{
    generateXml();
    parseBean();
}

void MainWindow::generateXml()
{
    using namespace Mc::XmlBuilder;
    BeanCollection bc;
    {
        auto bean = ClassBeanPtr::create("SimplePod", "simplePod");
        bc.addBean(bean);
    }
    {
        auto bean = ClassBeanPtr::create("SimpleGadget", "simpleGadget");
        bean->addContent(ConstructorArgPtr::create(0, "simpleGadget"));
        bean->addContent(PropertyPtr::create("text2", "simpleGadget2"));
        bc.addBean(bean);
    }
    {
        auto bean = ClassBeanPtr::create("SimpleGadgetPointer", "simpleGadgetPointer");
        bean->setPointer(true);
        bc.addBean(bean);
    }
    {
        auto bean = ClassBeanPtr::create("SimpleObject", "simpleObject");
        bc.addBean(bean);
    }
    {
#ifdef Q_OS_WIN
        auto bean = PluginBeanPtr::create("./PluginExample.dll", "simplePlugin");
#else
        auto bean = PluginBeanPtr::create("../lib/libPluginExample.so", "simplePlugin");
#endif
        auto checker = PluginCheckerPtr::create();
        checker->setClassName(QLatin1String("PluginChecker"));
        bean->addContent(checker);
        bc.addBean(bean);
    }
    {
#ifdef Q_OS_WIN
        auto bean = PluginBeanPtr::create("./PluginExample2.dll", "simplePlugin2");
#else
        auto bean = PluginBeanPtr::create("../lib/libPluginExample2.so", "simplePlugin2");
#endif
        auto checker = PluginCheckerPtr::create();
        checker->setClassName(QLatin1String("PluginChecker"));
        bean->addContent(checker);
        bc.addBean(bean);
    }
    {
        auto bean = ClassBeanPtr::create("ExampleObject", "exampleObject");
        bean->addContent(PropertyPtr::create("text", "exampleObject"));
        bean->addContent(PropertyPtr::create("pod", RefPtr::create("simplePod")));
        bean->addContent(PropertyPtr::create("gadget", RefPtr::create("simpleGadget")));
        bean->addContent(PropertyPtr::create("gadgetPointer", RefPtr::create("simpleGadgetPointer")));
        auto list = ListPtr::create();
        list->addContent(RefPtr::create("simpleObject"));
        list->addContent(RefPtr::create("simplePlugin"));
        list->addContent(RefPtr::create("simplePlugin2"));
        bean->addContent(PropertyPtr::create("list", list));
        auto map = MapPtr::create();
        map->addContent(QLatin1String("rose"), QLatin1String("jack"));
        bean->addContent(PropertyPtr::create("map", map));
        bc.addBean(bean);
    }
    {
        auto bean = ClassBeanPtr::create("List", "list");
        auto list = ListPtr::create();
        list->addContent(RefPtr::create("simpleObject"));
        list->addContent(RefPtr::create("simplePlugin"));
        list->addContent(RefPtr::create("simplePlugin2"));
        list->addContent(RefPtr::create("exampleObject"));
        bean->addContent(list);
        bc.addBean(bean);
    }
    {
        auto bean = ClassBeanPtr::create("Map", "map");
        auto map = MapPtr::create();
        map->addContent(PlaceholderPtr::create("custom"), RefPtr::create("simpleObject"));
        map->addContent(PlaceholderPtr::create("custom"), RefPtr::create("simplePlugin"));
        map->addContent(PlaceholderPtr::create("custom"), RefPtr::create("simplePlugin2"));
        map->addContent(PlaceholderPtr::create("custom"), RefPtr::create("exampleObject"));
        bean->addContent(map);
        bc.addBean(bean);
    }

    QFile file(Mc::toAbsolutePath("./IocExample.xml"));
    if (!file.open(QIODevice::ReadWrite | QIODevice::Truncate)) {
        return;
    }
    bc.writeToDevice(&file);
}

void MainWindow::parseBean()
{
    auto appCtx = McLocalPathApplicationContextPtr::create("./IocExample.xml");
    //    appCtx->refresh();
    auto object = appCtx->getBean<SimpleInterfacePtr>("exampleObject");
    object->simpleFunc();
    auto list = appCtx->getBean<QList<SimpleInterfacePtr>>("list");
    qDebug() << "list start ==============================================================";
    for (auto v : list) {
        qDebug() << v;
        v->simpleFunc();
    }
    qDebug() << "list end ==============================================================";
    auto map = appCtx->getBean<QMap<QString, SimpleInterfacePtr>>("map");
    qDebug() << "map start ==============================================================";
    QMapIterator<QString, SimpleInterfacePtr> itr(map);
    while (itr.hasNext()) {
        auto item = itr.next();
        qDebug() << item.key() << item.value();
        item.value()->simpleFunc();
    }
    qDebug() << "map end ==============================================================";
}
