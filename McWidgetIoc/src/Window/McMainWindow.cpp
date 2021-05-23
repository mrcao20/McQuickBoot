#include "McWidgetIoc/Window/McMainWindow.h"

#include "framelesswindowsmanager.h"

MC_STATIC()
auto pid = qRegisterMetaType<McMainWindow *>();
McPrivate::AddQObjectBuilderHelper<McMainWindow *, true>::addQObjectBuilder(pid);
MC_STATIC_END

MC_DECL_PRIVATE_DATA(McMainWindow)
bool isInited{false};
MC_DECL_PRIVATE_DATA_END

McMainWindow::McMainWindow(QWidget *parent, Qt::WindowFlags flags) : QMainWindow(parent, flags)
{
    MC_NEW_PRIVATE_DATA(McMainWindow);
}

McMainWindow::~McMainWindow() {}

void McMainWindow::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    if (!d->isInited) {
        FramelessWindowsManager::addWindow(windowHandle());
        d->isInited = true;
    }
}
