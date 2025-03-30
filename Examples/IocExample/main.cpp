#include <QApplication>

#include <McCore/McGlobal.h>

#include "MainWindow.h"

MC_STATIC()
#ifdef Q_OS_WIN
Mc::loadLibrary(QString("./LibraryExample.dll"), QLatin1String());
#else
Mc::loadLibrary(QString("../lib/libLibraryExample.so"), QLatin1String());
#endif
MC_STATIC_END

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow w;
    w.show();
    return app.exec();
}
