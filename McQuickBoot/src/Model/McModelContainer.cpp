#include "McBoot/Model/McModelContainer.h"

#include <QQmlEngine>
#include <QtDebug>

#include <McIoc/ApplicationContext/IMcApplicationContext.h>

#include "McBoot/IMcQuickBoot.h"

MC_INIT(McModelContainer)
MC_REGISTER_BEAN_FACTORY(McModelContainer)
MC_INIT_END

MC_DECL_PRIVATE_DATA(McModelContainer)
MC_DECL_PRIVATE_DATA_END

McModelContainer::McModelContainer(QObject *parent)
    : QObject(parent)
{
}

McModelContainer::~McModelContainer() 
{
}

void McModelContainer::init(IMcQuickBootConstPtrRef boot) noexcept 
{
    auto appCtx = boot->getApplicationContext();
    auto beanNames = Mc::getComponents(appCtx, MC_MODEL_TAG);
    for (const auto &beanName : beanNames) {
        auto obj = appCtx->getBean(beanName);
        if(!obj) {
            qCritical() << QString("model for named '%1' not exists").arg(beanName);
            continue;
        }
        QQmlEngine::setObjectOwnership(obj.data(), QQmlEngine::CppOwnership);
    }
}
