#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <McCore/Callback/Impl/McCppAsyncCallback.h>
#include <McCore/Callback/Impl/McCppSyncCallback.h>
#include <McCore/Config/McConfigNode.h>
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

MC_INIT2(MainWindow)
{
    qDebug() << "test MC_INIT2";
}
//!<

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //! 公共事件分发器
    Mc::eventDispatcher().connectToEvent("button.click", this, [](const QVariant &var) { qDebug() << var; });
    Mc::eventDispatcher().connectToEvent("button.click", this, [](double var) { qDebug() << var; });
    Mc::eventDispatcher().connectToEvent("button.click", []() { qDebug() << "button.click not receive arg"; });
    Mc::eventDispatcher().connectToEvent("button.click", this, &MainWindow::eventProcess);
    Mc::eventDispatcher().connectToEvent("button.click", this, SLOT(eventProcess2(QVariantList)));
    Mc::eventDispatcher().connectToEvent("enum.meta", this, [](Qt::ScrollBarPolicy e) { qDebug() << "enum.meta" << e; });
    Mc::eventDispatcher().connectToEvent(
        "enum.metaFlag", this, [](Qt::Alignment f) { qDebug() << "enum.metaFlag" << f; });
    Mc::eventDispatcher().connectToEvent(
        "enum.plain", this, [](QWidget::RenderFlag e) { qDebug() << "enum.plain" << e; });
    Mc::eventDispatcher().connectToEvent(
        "enum.plainFlag", this, [](QWidget::RenderFlags f) { qDebug() << "enum.plain" << f; });
    //!<

    //! 回调函数
    McCppSyncCallback callback([]() { qDebug() << "callback1"; });
    qDebug() << "callback1 return:" << callback.call(new QMainWindow(this));

    McCppSyncCallback callback2(this, [](int i) { qDebug() << "callback2" << i; });

    IMcCallbackPtr callback3 = McCppSyncCallback::build(
        this, [](const QString &msg) { qDebug() << "callback3" << msg; });
    callback3->call("test3");
    ///////////////////////////////////////////////////
    McCppAsyncCallback asyncCallback([]() -> QString {
        qDebug() << "asyncCallback";
        return "asyncCallback return";
    });
    qDebug() << asyncCallback.call<QString>("test");

    McCppAsyncCallback asyncCallback2([](int i) { qDebug() << "asyncCallback2" << i; });

    IMcCallbackPtr asyncCallback3 = McCppAsyncCallback::build(
        [](const QString &msg) { qDebug() << "asyncCallback3" << msg; });
    asyncCallback3->call("test3");
    //!<

    connect(ui->pushButton, &QPushButton::clicked, this, [callback2, asyncCallback2]() {
        static int i = 1;
        qDebug() << "callback2 return:" << callback2(i);
        asyncCallback2(i);
        mcEvt().submitEvent("button.click", i++);

        Mc::eventDispatcher().submitEvent("enum.meta", Qt::ScrollBarAsNeeded);
        Mc::eventDispatcher().submitEvent("enum.metaFlag", Qt::Alignment(Qt::AlignLeft));
        Mc::eventDispatcher().submitEvent("enum.plain", QWidget::DrawWindowBackground);
        Mc::eventDispatcher().submitEvent("enum.plainFlag", QWidget::RenderFlags(QWidget::DrawWindowBackground));
    });
    connect(ui->btnCrash, &QPushButton::clicked, this, &MainWindow::crashTrigger);

    {
        qDebug() << "===================== config test =====================";
        mcRegisterContainer<QMap<QString, int>>();
        if (!Mc::globalConfig()["aaa"].isDefined()) {
            QMap<QString, int> map;
            map.insert("bbb", 1);
            map.insert("ccc", 2);
            Mc::globalConfig()["aaa"] = map;
        }
        QMap<QString, int> pair = Mc::globalConfig()["aaa"].asMap<QMap<QString, int>>();
        qDebug() << pair << Mc::globalConfig()["aaa"].isPair();

        auto keys = Mc::globalConfig()["aaa"].keys();
        qDebug() << keys;
        qDebug() << Mc::globalConfig()["aaa"].at(0).key() << Mc::globalConfig()["aaa"].at(0).value().as<QString>();
        qDebug() << Mc::globalConfig()["aaa"][keys.constLast()].as<QString>();

        for (auto itr = Mc::globalConfig()["aaa"].constBegin(), end = Mc::globalConfig()["aaa"].constEnd(); itr != end;
             ++itr) {
            qDebug() << (*itr).key() << (*itr).value().as<QString>();
        }
        qDebug() << "===================== config test end =====================";
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::eventProcess(int val)
{
    qDebug() << "eventProcess" << val;
}

void MainWindow::eventProcess2(const QVariantList &varList)
{
    qDebug() << "eventProcess2" << varList;
}

void MainWindow::crashTrigger()
{
    int *ptr = nullptr;
    *ptr = 1;
}
