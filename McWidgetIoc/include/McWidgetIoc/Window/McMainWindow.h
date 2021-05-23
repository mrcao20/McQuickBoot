#pragma once

#include <QMainWindow>

#include <McIoc/McGlobal.h>

MC_FORWARD_DECL_PRIVATE_DATA(McMainWindow)

class McMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit McMainWindow(QWidget *parent = nullptr, Qt::WindowFlags flags = {});
    ~McMainWindow() override;

protected:
    void showEvent(QShowEvent *event) override;

private:
    MC_DECL_PRIVATE(McMainWindow)
};
