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

#include "../IMcLoggerRepository.h"
#include <McIoc/Thread/IMcDeleteThreadWhenQuit.h>

MC_FORWARD_DECL_CLASS(IMcAdditionalTask);

MC_FORWARD_DECL_PRIVATE_DATA(McLoggerRepository);

class MCLOGQT_EXPORT McLoggerRepository 
        : public QObject
        , public IMcDeleteThreadWhenQuit
        , public IMcLoggerRepository 
{
    Q_OBJECT
    MC_DECL_INIT(McLoggerRepository)
    MC_TYPELIST(QObject, IMcLoggerRepository, IMcDeleteThreadWhenQuit)
    typedef QMap<QString, IMcLoggerPtr> LoggerMap;
    Q_PROPERTY(LoggerMap loggers READ loggers WRITE setLogger)
    Q_PRIVATE_PROPERTY(d, IMcLoggerPtr notCapturedLogger MEMBER notCapturedLogger)
    Q_PRIVATE_PROPERTY(d, int taskTimeout MEMBER taskTimeout)
    Q_PRIVATE_PROPERTY(d, QList<IMcAdditionalTaskPtr> parallelTasks MEMBER parallelTasks)
    Q_PRIVATE_PROPERTY(d, QList<IMcAdditionalTaskPtr> sequentialTasks MEMBER sequentialTasks)
public:
    Q_INVOKABLE McLoggerRepository();
    ~McLoggerRepository() override;
    
    QMap<QString, IMcLoggerPtr> loggers() const noexcept override;
    void addLogger(const QString &loggerName, IMcLoggerConstPtrRef logger) noexcept override;
    void setLogger(const QMap<QString, IMcLoggerPtr> &loggers) noexcept override;
    IMcLoggerPtr getLogger(const QString &loggerName) noexcept override;
    void runTask() noexcept override;

    void deleteWhenQuit() noexcept override;

    Q_INVOKABLE
    MC_BEAN_FINISHED
    void finished() noexcept;

    Q_INVOKABLE
    MC_ALL_FINISHED
    void allFinished() noexcept;

private slots:
    void executeTasks() noexcept;

private:
    Q_INVOKABLE void processEvents() noexcept;
    
private:
    MC_DECL_PRIVATE(McLoggerRepository)
};

MC_DECL_METATYPE(McLoggerRepository)
