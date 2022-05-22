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
#pragma once

#include <McIoc/Utils/IMcBeanBuildable.h>

#include "../IMcLoggerRepository.h"

MC_FORWARD_DECL_CLASS(IMcAdditionalTask)

MC_FORWARD_DECL_PRIVATE_DATA(McLoggerRepository)

class MC_LOG_EXPORT McLoggerRepository
    : public QObject
    , public IMcLoggerRepository
    , public IMcBeanBuildable
{
    Q_OBJECT
    MC_FULL_DEFINE(McLoggerRepository, QObject, IMcLoggerRepository)
    Q_PROPERTY(QMap<QString, IMcLoggerPtr> loggers READ loggers WRITE setLogger)
    Q_PRIVATE_PROPERTY(d, IMcLoggerPtr uncapturedLogger MEMBER uncapturedLogger)
    Q_PRIVATE_PROPERTY(d, int taskTimeout MEMBER taskTimeout)
    Q_PRIVATE_PROPERTY(d, QList<IMcAdditionalTaskPtr> sequentialTasks MEMBER sequentialTasks)
    Q_PRIVATE_PROPERTY(d, bool flushWhenQuit MEMBER flushWhenQuit)
    Q_PRIVATE_PROPERTY(d, bool waitThreadFinished MEMBER waitThreadFinished)
    Q_PRIVATE_PROPERTY(d, qint64 threadWaitTimeout MEMBER threadWaitTimeout)
public:
    McLoggerRepository() noexcept;
    ~McLoggerRepository();

    QMap<QString, IMcLoggerPtr> loggers() const noexcept override;
    void addLogger(const QString &loggerName, const IMcLoggerPtr &logger) noexcept override;
    void setLogger(const QMap<QString, IMcLoggerPtr> &loggers) noexcept override;
    IMcLoggerPtr getLogger(const QString &loggerName) noexcept override;
    void runTask() noexcept override;
    void flushWhenQuit() noexcept override;

    void buildFinished() noexcept override;
    void buildCompleted() noexcept override;

private:
    void executeTasks() noexcept;
    void processEvents() noexcept;

private:
    MC_DECL_PRIVATE(McLoggerRepository)
};

MC_DECL_POINTER(McLoggerRepository)
