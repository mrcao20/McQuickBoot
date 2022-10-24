#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QThread>

#include <McQuickBoot/Invoker/McInvoker.h>
#include <McQuickBoot/Invoker/McResult.h>

MC_STATIC()
MC_STATIC_END

void aaa()
{
    throw std::exception("aaa exception");
}

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
    Mc::invoke(functor);
    Mc::invoke(functor, "invoke1").then([](int i) { qDebug() << "invoke return 1:" << i << QThread::currentThread(); });
    Mc::invoke(this, &MainWindow::func);
    Mc::invoke(this, &MainWindow::func, "aaa");
    Mc::invoke(&aaa).onError([](const McResultPtr &r) { qDebug() << "aaa error:" << r; });
    Mc::invoke(&aaa, "vvv");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::func()
{
    qDebug() << "MainWindow func:" << QThread::currentThread();
}
