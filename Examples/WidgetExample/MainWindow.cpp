#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QLabel>
#include <QStackedWidget>

#include <McWidget/ApplicationContext/Impl/McLocalPathWidgetApplicationContext.h>

MC_STATIC()
mcRegisterMetaTypeSimple<QLabel>();
mcRegisterMetaTypeSimple<CustomWidget>();
MC_STATIC_END

CustomWidget::CustomWidget()
{
    new QLabel("CustomWidget", this);
}

CustomWidget::~CustomWidget()
{
    qDebug() << "~CustomWidget";
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    auto appCtx = McLocalPathWidgetApplicationContextPtr::create(":/widgetTest.xml");
    appCtx->refresh();

    auto w = appCtx->getBean<QStackedWidget *>(QLatin1String("stackWidget"));
    ui->verticalLayout->addWidget(w);
    connect(ui->pushButton, &QAbstractButton::clicked, this, [w]() {
        int index = w->currentIndex();
        index += 1;
        index %= 2;
        w->setCurrentIndex(index);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}
