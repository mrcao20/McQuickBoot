#include "McWidgetIoc/Window/McWidget.h"

#include "framelesswindowsmanager.h"

MC_STATIC()
auto pid = qRegisterMetaType<McWidget *>();
McPrivate::AddQObjectBuilderHelper<McWidget *, true>::addQObjectBuilder(pid);
MC_STATIC_END

MC_DECL_PRIVATE_DATA(McWidget)
bool isInited{false};
MC_DECL_PRIVATE_DATA_END

McWidget::McWidget(QWidget *parent) : QWidget(parent)
{
    MC_NEW_PRIVATE_DATA(McWidget);
}

McWidget::~McWidget() {}

void McWidget::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    if (!d->isInited) {
        FramelessWindowsManager::addWindow(windowHandle());
        d->isInited = true;
    }
}
