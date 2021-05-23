#include <QApplication>
#include <QWidget>

#include <McBoot/McQuickBootSimple.h>

#include <McWidgetIoc/ApplicationContext/Impl/McXmlWidgetApplicationContext.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    McQuickBootSimple::init();
    auto appCtx = McXmlWidgetApplicationContextPtr::create(
        R"(../../Examples/WidgetIocTest/widgetioc.xml)");
    auto w = appCtx->getBean("w");
    Mc::addPostRoutine(0, [w]() { delete w; });
    w->show();
    return a.exec();
}
