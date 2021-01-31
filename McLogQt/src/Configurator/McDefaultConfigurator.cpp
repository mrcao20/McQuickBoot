#include "McLog/Configurator/McDefaultConfigurator.h"

#include <QThread>

#include "McLog/McLogManager.h"
#include "McLog/Repository/impl/McLoggerRepository.h"

void McDefaultConfigurator::configure() noexcept
{
    QThread *thread = new QThread(); //!< 此线程将在LoggerRepository被析构时退出和销毁
    thread->start();

    auto rep = McLoggerRepositoryPtr::create();
    rep->moveToThread(thread);
    rep->finished();
    rep->deleteWhenQuit();
    QMetaObject::invokeMethod(rep.data(), MC_STRINGIFY(allFinished), Qt::BlockingQueuedConnection);
    McLogManager::instance()->setLoggerRepository(rep);

    McLogManager::installQtMessageHandler();
}
