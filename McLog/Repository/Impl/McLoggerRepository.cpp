/*
 * MIT License
 *
 * Copyright (c) 2021 mrcao20
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "McLoggerRepository.h"

#include <QAbstractEventDispatcher>
#include <QScopeGuard>
#include <QThread>
#include <QTimer>

#include "Appender/Impl/McConsoleAppender.h"
#include "Logger/Impl/McLogger.h"
#include "Repository/IMcAdditionalTask.h"

MC_DECL_PRIVATE_DATA(McLoggerRepository)
QMap<QString, IMcLoggerPtr> loggers;
IMcLoggerPtr uncapturedLogger;
QThread *thread{nullptr};
int taskTimeout{3600000};
QList<IMcAdditionalTaskPtr> sequentialTasks;
QTimer taskTimer;
bool flushWhenQuit{false};
bool waitThreadFinished{true};
qint64 threadWaitTimeout{-1};
MC_DECL_PRIVATE_DATA_END

MC_AUTO_INIT(McLoggerRepository)
mcRegisterContainer<QMap<QString, IMcLoggerPtr>>();
mcRegisterContainer<QList<IMcAdditionalTaskPtr>>();
MC_INIT_END

McLoggerRepository::McLoggerRepository() noexcept
{
    MC_NEW_PRIVATE_DATA(McLoggerRepository);

    //! 回归到当前线程执行
    connect(&d->taskTimer, &QTimer::timeout, this, &McLoggerRepository::executeTasks, Qt::QueuedConnection);
}

McLoggerRepository::~McLoggerRepository()
{
    if (QThread::currentThread() == thread()) {
        processEvents();
    } else {
        QMetaObject::invokeMethod(this, &McLoggerRepository::processEvents, Qt::BlockingQueuedConnection);
    }
    if (d->thread == nullptr) {
        return;
    }
    auto cleanup = qScopeGuard([this]() { MC_SAFETY_DELETE(d->thread); });
    d->thread->quit();
    if (!d->waitThreadFinished) {
        return;
    }
    QDeadlineTimer deadlineTimer;
    if (d->threadWaitTimeout == -1) {
        deadlineTimer = QDeadlineTimer(QDeadlineTimer::Forever);
    } else {
        deadlineTimer = QDeadlineTimer(d->threadWaitTimeout);
    }
    if (d->thread->thread() != QThread::currentThread()) {
        d->thread->wait(deadlineTimer);
    } else {
        while ((!d->thread->isFinished() || d->thread->isRunning()) && !deadlineTimer.hasExpired()) {
            QThread::msleep(100);
        }
    }
}

QMap<QString, IMcLoggerPtr> McLoggerRepository::loggers() const noexcept
{
    return d->loggers;
}

void McLoggerRepository::addLogger(const QString &loggerName, const IMcLoggerPtr &logger) noexcept
{
    d->loggers.insert(loggerName, logger);
}

void McLoggerRepository::setLogger(const QMap<QString, IMcLoggerPtr> &loggers) noexcept
{
    d->loggers = loggers;
}

IMcLoggerPtr McLoggerRepository::getLogger(const QString &loggerName) noexcept
{
    if (!d->loggers.contains(loggerName)) {
        return d->uncapturedLogger;
    }
    return d->loggers.value(loggerName);
}

void McLoggerRepository::runTask() noexcept
{
    if (QThread::currentThread() == thread()) {
        executeTasks();
    } else {
        QMetaObject::invokeMethod(this, &McLoggerRepository::executeTasks, Qt::BlockingQueuedConnection);
    }
}

void McLoggerRepository::flushWhenQuit() noexcept
{
    if (!d->flushWhenQuit) {
        return;
    }
    auto categories = d->loggers.keys();
    for (const auto &category : qAsConst(categories)) {
        qDebug(QLoggingCategory(category.toLocal8Bit()));
        qInfo(QLoggingCategory(category.toLocal8Bit()));
        qWarning(QLoggingCategory(category.toLocal8Bit()));
        qCritical(QLoggingCategory(category.toLocal8Bit()));
    }
}

void McLoggerRepository::buildFinished() noexcept
{
    d->taskTimer.start(d->taskTimeout);
}

void McLoggerRepository::buildCompleted() noexcept
{
    d->thread = thread();
    if (d->uncapturedLogger.isNull()) {
        auto logger = McLoggerPtr::create();
        QList<IMcConfigurableAppenderPtr> appenders;
        auto consoleAppender = McConsoleAppenderPtr::create();
        consoleAppender->setImmediateFlush(true);
        consoleAppender->buildFinished();
        consoleAppender->buildThreadMoved();
        consoleAppender->buildCompleted();
        appenders.append(consoleAppender);
        logger->setAppenders(appenders);
        logger->buildFinished();
        d->uncapturedLogger = logger;
    }
    QTimer::singleShot(std::chrono::milliseconds(1000), this, &McLoggerRepository::executeTasks);
}

void McLoggerRepository::executeTasks() noexcept
{
    for (auto &sequentialTask : qAsConst(d->sequentialTasks)) {
        sequentialTask->execute();
    }
}

void McLoggerRepository::processEvents() noexcept
{
    thread()->eventDispatcher()->processEvents(QEventLoop::AllEvents);
}

#include "moc_McLoggerRepository.cpp"
