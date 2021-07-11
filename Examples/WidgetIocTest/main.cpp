#include <QApplication>

#include <McBoot/McQuickBootSimple.h>

template<typename T>
struct AAA
{};

template<typename T>
using AAAPtr = QSharedPointer<AAA<T>>;

int main(int argc, char *argv[])
{
    qDebug() << AAAPtr<int>::create();
    McQuickBootSimple::addConfigPath(R"(../../Examples/WidgetIocTest/application.yml)");
    QApplication a(argc, argv);
    McQuickBootSimple::init();
    return a.exec();
}
