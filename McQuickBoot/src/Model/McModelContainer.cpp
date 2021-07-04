#include "McBoot/Model/impl/McModelContainer.h"

#include <QDebug>
#include <QQmlEngine>

#include <McIoc/ApplicationContext/IMcApplicationContext.h>

#include "McBoot/IMcQuickBoot.h"

MC_INIT(McModelContainer)
MC_REGISTER_BEAN_FACTORY(McModelContainer)
MC_INIT_END

MC_DECL_PRIVATE_DATA(McModelContainer)
QMap<QString, QObjectPtr> models; //!< 键为beanName，值为model对象
MC_DECL_PRIVATE_DATA_END

McModelContainer::McModelContainer(QObject *parent)
    : QObject(parent)
{
    MC_NEW_PRIVATE_DATA(McModelContainer);
}

McModelContainer::~McModelContainer() 
{
}

void McModelContainer::init(const IMcQuickBoot *boot) noexcept
{
    d->models.clear();
    auto appCtx = boot->getApplicationContext();
    auto beanNames = Mc::getComponents(appCtx, MC_MODEL_TAG);
    for (const auto &beanName : beanNames) {
        auto obj = appCtx->getBean(beanName);
        if(!obj) {
            qCritical() << QString("model for named '%1' not exists").arg(beanName);
            continue;
        }
        if (d->models.contains(beanName)) {
            qCritical() << QString("model for named '%1' is repeated").arg(beanName);
            continue;
        }
        QQmlEngine::setObjectOwnership(obj.data(), QQmlEngine::CppOwnership);
        d->models.insert(beanName, obj);
    }
}

QObject *McModelContainer::getModel(const QString &beanName) noexcept
{
    return d->models.value(beanName, QObjectPtr()).data();
}
