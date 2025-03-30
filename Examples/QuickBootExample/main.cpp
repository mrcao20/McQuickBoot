#include <QApplication>

#include <McQuickBoot/QuickBoot/McQuickBoot.h>

#include "MainWindow.h"

// #undef MC_ENABLE_QML

int main(int argc, char *argv[])
{
    McQuickBoot::setPreInitFunc([](QCoreApplication *) {
        qDebug() << QStringLiteral(u"此代码将在QGuiApplication构造完成后，框架初始化之前调用");
    });
    McQuickBoot::setAfterInitFunc([](QCoreApplication *, QQmlApplicationEngine *) {
        qDebug() << QStringLiteral(u"此代码将在QGuiApplication构造完成后并且框架初始化之后调用");
    });
#ifdef MC_ENABLE_QML
    return McQuickBoot::run(argc, argv);
#else
    return McQuickBoot::run<QApplication, MainWindow>(argc, argv);
#endif
}
