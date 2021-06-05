#include <QApplication>

#include <McBoot/McQuickBootSimple.h>

int main(int argc, char *argv[])
{
    McQuickBootSimple::addConfigPath(R"(../../Examples/WidgetIocTest/application.yml)");
    QApplication a(argc, argv);
    McQuickBootSimple::init();
    return a.exec();
}
