#include "McBoot/Configuration/McWidgetConfig.h"

#include <QWidget>

#include <McWidgetIoc/ApplicationContext/Impl/McXmlWidgetApplicationContext.h>

MC_STATIC()
MC_REGISTER_BEAN_FACTORY(McWidgetConfig)
MC_STATIC_END

MC_DECL_PRIVATE_DATA(McWidgetConfig)
QStringList xmlPaths;
QString mainWindowName{"mainWindow"};
QString flag;
int destroyPriority{0};
MC_DECL_PRIVATE_DATA_END

McWidgetConfig::McWidgetConfig(QObject *parent) noexcept : QObject(parent)
{
    MC_NEW_PRIVATE_DATA(McWidgetConfig);
}

McWidgetConfig::~McWidgetConfig() {}

void McWidgetConfig::allFinished() noexcept
{
    if (d->xmlPaths.isEmpty()) {
        return;
    }
    auto appCtx = McXmlWidgetApplicationContextPtr::create(d->xmlPaths, d->flag);
    auto w = appCtx->getBean(d->mainWindowName);
    if (w == nullptr) {
        return;
    }
    Mc::addPostRoutine(d->destroyPriority, [w]() { delete w; });
    w->show();
}

#include "moc_McWidgetConfig.cpp"
