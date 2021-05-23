#pragma once

#include <QWidget>

#include <McIoc/McGlobal.h>

MC_FORWARD_DECL_PRIVATE_DATA(McWidget)

class McWidget : public QWidget
{
    Q_OBJECT
public:
    explicit McWidget(QWidget *parent = nullptr);
    ~McWidget() override;

protected:
    void showEvent(QShowEvent *event) override;

private:
    MC_DECL_PRIVATE(McWidget)
};
