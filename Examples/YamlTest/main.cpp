#include "MainWindow.h"
#include <QApplication>
#include <QSettings>
#include <QDebug>
#include "McYaml/McYaml.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    McYaml::registerYaml();
    
    auto path = qApp->applicationDirPath() + "/logqt.yml";
    QSettings set(path, McYaml::format());
    set.beginGroup("logger");
    set.beginGroup("default");
    set.setValue("a", "b");
    set.endGroup();
    set.endGroup();
    
    MainWindow w;
    w.show();
    
    return a.exec();
}
