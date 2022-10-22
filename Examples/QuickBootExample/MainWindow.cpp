#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QThread>

#include <McQuickBoot/Invoker/McInvoker.h>

MC_STATIC()
MC_STATIC_END

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->pushButton, &QAbstractButton::clicked, this, []() {});

    auto functor = [](const QString &str) {
        qDebug() << "invoke:" << str << QThread::currentThread();
        return 1;
    };
    Mc::invoke(functor, "invoke1").then([](int i) { qDebug() << "invoke return 1:" << i << QThread::currentThread(); });
}

MainWindow::~MainWindow()
{
    delete ui;
}
