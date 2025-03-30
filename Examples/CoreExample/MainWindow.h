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

private Q_SLOTS:
    void eventProcess(int val);
    void eventProcess2(const QVariantList &varList);

    void crashTrigger();

private:
    Ui::MainWindow *ui;
};
