#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QThread>

#include "McQuickBoot/QuickBoot/McQuickBoot.h"
#include <McQuickBoot/Invoker/McInvoker.h>
#include <McQuickBoot/Invoker/McResult.h>

MC_STATIC()
MC_STATIC_END

void aaa()
{
#ifdef Q_CC_MSVC
    throw std::exception("aaa exception");
#else
    throw std::exception();
#endif
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
    Mc::invoke(this, qOverload<>(&MainWindow::func));
    Mc::invoke(this, qOverload<const QString &>(&MainWindow::func), "aaa");
    Mc::invoke(&aaa).onError([](const McResultPtr &r) { qDebug() << "aaa error:" << r; });
    Mc::invoke(&aaa, "vvv");

    $.invoke("testController.testMethod").then([](const QString &str) { qDebug() << str; });
    $.invoke("testController.testMethod2", "bbb");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::func()
{
    qDebug() << "MainWindow func:" << QThread::currentThread();
}

void MainWindow::func(const QString &parm)
{
    qDebug() << "MainWindow func parm:" << parm << QThread::currentThread();
}
