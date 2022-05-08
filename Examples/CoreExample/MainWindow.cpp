#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <McCore/Event/McEventDispatcher.h>
#include <McCore/McGlobal.h>

//! 模拟静态代码块
MC_STATIC()
qDebug() << "static block normal level";
//! 路径占位符注册和使用
Mc::registerPathPlaceholder("{custom}/bin", []() { return QCoreApplication::applicationDirPath(); });
qDebug() << "core example init: " << Mc::toAbsolutePath("{custom}/bin");
qDebug() << "core example init: " << Mc::toAbsolutePath("{appData}/bin");
//!<
MC_DESTROY()
qDebug() << "core example destroy";
MC_STATIC_END
//!<

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //! 公共事件分发器
    Mc::eventDispatcher().connectToEvent("button.click", this, [](const QVariant &var) { qDebug() << var; });
    Mc::eventDispatcher().connectToEvent("button.click", this, [](int var) { qDebug() << var; });
    Mc::eventDispatcher().connectToEvent("button.click", []() { qDebug() << "button.click not receive arg"; });
    Mc::eventDispatcher().connectToEvent("button.click", this, &MainWindow::eventProcess);
    connect(ui->pushButton, &QPushButton::clicked, this, []() {
        static int i = 1;
        mcEvt().submitEvent("button.click", i++);
    });
    //!<
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::eventProcess(int val)
{
    qDebug() << "eventProcess" << val;
}
