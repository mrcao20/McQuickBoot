#include <QApplication>
#include <QDebug>

#include <McCore/McGlobal.h>

#include "MainWindow.h"

MC_STATIC(Mc::RoutinePriority::Normal + 1)
qDebug() << "static block high level";
MC_STATIC_END

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow w;
    w.show();
    return app.exec();
}
