#include "McBoot/Model/McModelContainer.h"

#include <QQmlEngine>
#include <QtDebug>

#include <McIoc/ApplicationContext/IMcApplicationContext.h>

#include "McBoot/McIocBoot.h"

MC_DECL_PRIVATE_DATA(McModelContainer)
MC_DECL_PRIVATE_DATA_END

McModelContainer::McModelContainer(QObject *parent)
    : QObject(parent)
{
}

McModelContainer::~McModelContainer() 
{
}

void McModelContainer::init(McIocBootConstPtrRef boot) noexcept 
{
    auto appCtx = boot->getApplicationContext();
	auto beanNames = boot->getComponents(MC_MODEL_TAG);
    for (const auto &beanName : beanNames) {
        auto obj = appCtx->getBean(beanName);
        if(!obj) {
            qCritical() << QString("model for named '%1' not exists").arg(beanName);
            continue;
        }
        QQmlEngine::setObjectOwnership(obj.data(), QQmlEngine::CppOwnership);
    }
}
