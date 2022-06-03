#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <McCore/Callback/Impl/McCppAsyncCallback.h>
#include <McCore/Callback/Impl/McCppSyncCallback.h>
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
    //!<

    //! 回调函数
    McCppSyncCallback callback([]() { qDebug() << "callback1"; });
    callback.call("test");

    McCppSyncCallback callback2([](int i) { qDebug() << "callback2" << i; });

    IMcCallbackPtr callback3 = McCppSyncCallback::build([](const QString &msg) { qDebug() << "callback3" << msg; });
    callback3->call("test3");
    ///////////////////////////////////////////////////
    McCppAsyncCallback asyncCallback([]() { qDebug() << "asyncCallback"; });
    asyncCallback.call("test");

    McCppAsyncCallback asyncCallback2([](int i) { qDebug() << "asyncCallback2" << i; });

    IMcCallbackPtr asyncCallback3 = McCppAsyncCallback::build(
        [](const QString &msg) { qDebug() << "asyncCallback3" << msg; });
    asyncCallback3->call("test3");
    //!<

    connect(ui->pushButton, &QPushButton::clicked, this, [callback2, asyncCallback2]() {
        static int i = 1;
        callback2(i);
        asyncCallback2(i);
        mcEvt().submitEvent("button.click", i++);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::eventProcess(int val)
{
    qDebug() << "eventProcess" << val;
}
