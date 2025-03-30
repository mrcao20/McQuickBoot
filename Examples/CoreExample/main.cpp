#include <QApplication>
#include <QDebug>

#include <McCore/McGlobal.h>
#include <McCore/Utils/McCrashHandler.h>

#include "MainWindow.h"

MC_STATIC(Mc::RoutinePriority::RoutineNormal + 1)
qDebug() << "static block high level";
MC_STATIC_END

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow w;
    w.show();
    McCrashHandler crashHandler(Mc::toAbsolutePath(QStringLiteral("./log")));
    crashHandler.enableBackup(Mc::toAbsolutePath(QStringLiteral("./log/backup")), QStringLiteral("%{time yyyy-MM}"),
        QStringLiteral("%{time yyyy-MM-dd_hh-mm-ss}.dmp"));
    return app.exec();
}
