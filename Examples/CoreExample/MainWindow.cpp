#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <McCore/Event/McEventDispatcher.h>
#include <McCore/McGlobal.h>

//! 模拟静态代码块
MC_STATIC()
//! 路径占位符注册和使用
Mc::registerPathPlaceholder("{custom}/bin", []() { return QCoreApplication::applicationDirPath(); });
qDebug() << "core example init: " << Mc::toAbsolutePath("{custom}/bin");
qDebug() << "core example init: " << Mc::toAbsolutePath("{appData}/bin");
//!<
MC_DESTROY()
qDebug() << "core example destroy";
MC_STATIC_END
//!<

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //! 公共事件分发器
    Mc::eventDispatcher().connectToEvent("button.click", this, [](const QVariant &var) { qDebug() << var; });
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
