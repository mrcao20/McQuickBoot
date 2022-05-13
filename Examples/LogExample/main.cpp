#include <QApplication>

#include <McLog/Configurator/McXMLConfigurator.h>

#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
#ifdef Q_OS_WIN
    McXMLConfigurator::configure(":/log.xml", "win");
#else
    McXMLConfigurator::configure(":/log.xml", "unix");
#endif
    MainWindow w;
    w.show();
    return app.exec();
}
