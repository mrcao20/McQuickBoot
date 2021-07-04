#include "McBoot/Service/impl/McServiceContainer.h"

#include <QThread>
#include <QTimer>

#include <McIoc/ApplicationContext/IMcApplicationContext.h>

#include "McBoot/IMcQuickBoot.h"
#include "McBoot/Service/IMcServiceLongLiveThread.h"
#include "McBoot/Service/IMcServiceTimer.h"

MC_AUTO_INIT(McServiceContainer)
MC_INIT_END

MC_DECL_PRIVATE_DATA(McServiceContainer)
QMap<QString, QObjectPtr> services; //!< 键为beanName，值为service对象
QList<QTimer *> timers;
QList<QThread *> liveThreads;
MC_DECL_PRIVATE_DATA_END

McServiceContainer::McServiceContainer() noexcept
{
    MC_NEW_PRIVATE_DATA(McServiceContainer);
}

McServiceContainer::~McServiceContainer()
{
    for (auto timer : qAsConst(d->timers)) {
        timer->stop();
        delete timer;
    }
    for (auto liveThread : qAsConst(d->liveThreads)) {
        liveThread->quit();
    }
    for (auto liveThread : qAsConst(d->liveThreads)) {
        liveThread->wait();
        delete liveThread;
    }
}

void McServiceContainer::init(const IMcQuickBoot *boot) noexcept
{
    d->services.clear();
    auto appCtx = boot->getApplicationContext();
    auto beanNames = Mc::getComponents(appCtx, MC_SERVICE_TAG);
    for (const auto &beanName : beanNames) {
        auto var = appCtx->getBeanToVariant(beanName);
        auto obj = var.value<QObjectPtr>();
        if (!obj) {
            qCritical() << QString("service for named '%1' not exists").arg(beanName);
            continue;
        }
        if (d->services.contains(beanName)) {
            qCritical() << QString("service for named '%1' is repeated").arg(beanName);
            continue;
        }
        d->services.insert(beanName, obj);
        parseTimer(var);
        parseLongLiveThread(var);
    }
}

void McServiceContainer::parseTimer(const QVariant &var) noexcept
{
    if (!var.canConvert<IMcServiceTimerPtr>()) {
        return;
    }
    auto obj = var.value<QObjectPtr>();
    auto serverTimer = var.value<IMcServiceTimerPtr>();
    QTimer *timer = new QTimer();
    timer->setInterval(serverTimer->interval());
    timer->callOnTimeout(
        obj.data(), [serverTimer]() { serverTimer->timeout(); }, Qt::QueuedConnection);
    timer->start();
    d->timers.append(timer);
}

void McServiceContainer::parseLongLiveThread(const QVariant &var) noexcept
{
    if (!var.canConvert<IMcServiceLongLiveThreadPtr>()) {
        return;
    }
    auto obj = var.value<QObjectPtr>();
    auto live = var.value<IMcServiceLongLiveThreadPtr>();
    QThread *thread = new QThread();
    thread->start();
    QMetaObject::invokeMethod(
        obj.data(),
        [obj, live, thread]() {
            obj->moveToThread(thread);
            QMetaObject::invokeMethod(
                obj.data(), [live]() { live->moveFinished(); }, Qt::QueuedConnection);
        },
        Qt::QueuedConnection);
    d->liveThreads.append(thread);
}
