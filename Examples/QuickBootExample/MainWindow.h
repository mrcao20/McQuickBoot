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
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void func();
    void func(const QString &parm);

    static void f() {}

private:
    Ui::MainWindow *ui;
};
