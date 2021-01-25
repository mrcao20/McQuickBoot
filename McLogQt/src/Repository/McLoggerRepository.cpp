#include "McLog/Repository/impl/McLoggerRepository.h"

#include <QAbstractEventDispatcher>
#include <QThread>
#include <QTimer>
#include <QtConcurrent>

#include "McLog/Appender/impl/McConsoleAppender.h"
#include "McLog/Logger/impl/McLogger.h"
#include "McLog/Repository/IMcAdditionalTask.h"

MC_DECL_PRIVATE_DATA(McLoggerRepository)
QMap<QString, IMcLoggerPtr> loggers;
IMcLoggerPtr notCapturedLogger;
QThread *thread{nullptr};
int taskTimeout{3600000};
QList<IMcAdditionalTaskPtr> parallelTasks;
QList<IMcAdditionalTaskPtr> sequentialTasks;
QTimer taskTimer;
MC_DECL_PRIVATE_DATA_END

MC_INIT(McLoggerRepository)
MC_REGISTER_BEAN_FACTORY(McLoggerRepository)
MC_REGISTER_MAP_CONVERTER(LoggerMap)
MC_REGISTER_LIST_CONVERTER(QList<IMcAdditionalTaskPtr>);
MC_INIT_END

McLoggerRepository::McLoggerRepository()
{
    MC_NEW_PRIVATE_DATA(McLoggerRepository);

    //! 回归到当前线程执行
    connect(&d->taskTimer,
            &QTimer::timeout,
            this,
            &McLoggerRepository::executeTasks,
            Qt::QueuedConnection);
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
        return d->notCapturedLogger;
    }
    return d->loggers.value(loggerName);
}

void McLoggerRepository::runTask() noexcept
{
    if (QThread::currentThread() == thread()) {
        executeTasks();
    } else {
        QMetaObject::invokeMethod(this, MC_STRINGIFY(executeTasks), Qt::BlockingQueuedConnection);
    }
}

void McLoggerRepository::deleteWhenQuit() noexcept
{
    d->thread = thread();
}

void McLoggerRepository::finished() noexcept
{
    d->taskTimer.start(d->taskTimeout);
}

void McLoggerRepository::allFinished() noexcept
{
    if (d->notCapturedLogger.isNull()) {
        auto logger = McLoggerPtr::create();
        QList<IMcConfigurableAppenderPtr> appenders;
        auto consoleAppender = McConsoleAppenderPtr::create();
        consoleAppender->finished();
        appenders.append(consoleAppender);
        logger->setAppenders(appenders);
        logger->finished();
        d->notCapturedLogger = logger;
    }
    QTimer::singleShot(std::chrono::milliseconds(1000), this, &McLoggerRepository::executeTasks);
}

void McLoggerRepository::executeTasks() noexcept
{
    for (auto &parallelTask : qAsConst(d->parallelTasks)) {
        QtConcurrent::run([parallelTask]() { parallelTask->execute(); });
    }
    for (auto &sequentialTask : qAsConst(d->sequentialTasks)) {
        sequentialTask->execute();
    }
}

void McLoggerRepository::processEvents() noexcept
{
    d->thread->eventDispatcher()->processEvents(QEventLoop::AllEvents);
}

#include "moc_McLoggerRepository.cpp"
