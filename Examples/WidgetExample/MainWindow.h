#pragma once

#include <QMainWindow>

#include <McCore/McGlobal.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class CustomWidget : public QWidget
{
    Q_OBJECT
public:
    CustomWidget();
    ~CustomWidget();
};

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
};
