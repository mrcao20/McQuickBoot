#include "MainWindow.h"

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQuickWidget>

#include <McBoot/McQuickBoot.h>

int main(int argc, char *argv[])
{
    QQuickWindow::setGraphicsApi(QSGRendererInterface::OpenGL);
    QApplication a(argc, argv);
    QQmlApplicationEngine engine;
    McQuickBoot::init(&a, &engine);
    MainWindow w;
    auto ww = McQuickBoot::createQuickWidget("qrc:/main.qml");
    w.addWidget(ww);
    w.show();
    return a.exec();
}
