#pragma once

#include <QMainWindow>

#include <McCore/McGlobal.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    MC_DECL_INIT(MainWindow)
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void eventProcess(int val);

private:
    Ui::MainWindow *ui;
};
