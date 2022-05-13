#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QtConcurrent>

#include <McLog/McLogMacroGlobal.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    qDebug() << "debug";
    qWarning() << "warning";
    qCritical() << "critical";
    qInfo() << "info";
    qInfo() << "中文";
    qInfo() << QStringLiteral("中文2");
    qInfo() << QStringLiteral(u"中文3");
    qDebug(MC_LOGGER("custom")) << "custom debug";
    qWarning(MC_LOGGER("custom")) << "custom warning";
    qCritical(MC_LOGGER("custom")) << "custom critical";
    qInfo(MC_LOGGER("custom")) << "custom info";
    qInfo(MC_LOGGER("TmpLogger")) << "tmp logger";
    qInfo(MC_LOGGER("TmpLogger2")) << "tmp logger";
    (void)QtConcurrent::run([]() {
        QThread::msleep(2000);
        auto tid = QThread::currentThreadId();
        for (int i = 0; i < 100; i++) {
            qDebug() << "tid:" << tid << " " << i;
        }
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}
