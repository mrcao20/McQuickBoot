#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QDebug>

#include <McCore/Event/McEventDispatcher.h>
#include <McCore/McGlobal.h>

MC_STATIC()
qDebug() << "core example init";
MC_STATIC_END

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
      , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    McEvt.connectToEvent("button.click", this, [](const QVariant &var) { qDebug() << var; });
    connect(ui->pushButton, &QPushButton::clicked, this, []() { McEvt.submitEvent("button.click", 1); });
}

MainWindow::~MainWindow()
{
    delete ui;
}
