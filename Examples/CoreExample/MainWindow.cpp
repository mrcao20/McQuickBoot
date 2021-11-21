#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QDebug>

#include <McCore/Event/McEventDispatcher.h>
#include <McCore/McGlobal.h>

MC_STATIC()
Mc::registerPathPlaceholder("{custom}/bin", []() { return QCoreApplication::applicationDirPath(); });
qDebug() << "core example init: " << Mc::toAbsolutePath("{custom}/bin");
qDebug() << "core example init: " << Mc::toAbsolutePath("{appData}/bin");
MC_STATIC_END

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
      , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Mc::eventDispatcher().connectToEvent("button.click", this, [](const QVariant &var) { qDebug() << var; });
    connect(ui->pushButton, &QPushButton::clicked, this, []() {
        static int i = 1;
        mcEvt().submitEvent("button.click", i++);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}
