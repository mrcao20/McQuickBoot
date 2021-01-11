#include "McLog/Repository/impl/McLoggerRepository.h"

#include <QThread>
#include <QAbstractEventDispatcher>

#include "McLog/Logger/IMcLogger.h"
#include "McLog/Utils/Deleter/IMcLogDeleter.h"
#include "McLog/Utils/Packager/IMcLogPackager.h"

MC_DECL_PRIVATE_DATA(McLoggerRepository)
QMap<QString, IMcLoggerPtr> loggers;
QThread *thread{nullptr};
IMcLogDeleterPtr logDeleter;
IMcLogPackagerPtr logPackager;
MC_DECL_PRIVATE_DATA_END

MC_INIT(McLoggerRepository)
MC_REGISTER_BEAN_FACTORY(McLoggerRepository)
MC_REGISTER_MAP_CONVERTER(LoggerMap)
MC_INIT_END

McLoggerRepository::McLoggerRepository()
{
    MC_NEW_PRIVATE_DATA(McLoggerRepository);
}

McLoggerRepository::~McLoggerRepository()
{
    if(d->thread) {
        if(QThread::currentThread() == thread()) {
            processEvents();
        } else {
            QMetaObject::invokeMethod(this, "processEvents", Qt::BlockingQueuedConnection);
        }
        d->thread->quit();
        if(d->thread->thread() != QThread::currentThread()) {
            d->thread->wait();
        } else {
            while(!d->thread->isFinished() || d->thread->isRunning()) {
                QThread::msleep(100);
            }
        }
        delete d->thread;
    }
}

QMap<QString, IMcLoggerPtr> McLoggerRepository::loggers() const noexcept
{
    return d->loggers;
}

void McLoggerRepository::addLogger(const QString &loggerName, IMcLoggerConstPtrRef logger) noexcept
{
    d->loggers.insert(loggerName, logger);
}

void McLoggerRepository::setLogger(const QMap<QString, IMcLoggerPtr> &loggers) noexcept
{
    d->loggers = loggers;
}

IMcLoggerPtr McLoggerRepository::getLogger(const QString &loggerName) noexcept
{
    if(!d->loggers.contains(loggerName)) {
        return IMcLoggerPtr();
    }
    return d->loggers.value(loggerName);
}

void McLoggerRepository::deleteWhenQuit() noexcept
{
    d->thread = thread();
}

void McLoggerRepository::processEvents() noexcept
{
    d->thread->eventDispatcher()->processEvents(QEventLoop::AllEvents);
}

#include "moc_McLoggerRepository.cpp"
