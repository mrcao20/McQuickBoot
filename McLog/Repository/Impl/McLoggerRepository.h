/*
 * Copyright (c) 2021 mrcao20/mrcao20@163.com
 * McQuickBoot is licensed under Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at:
 *          http://license.coscl.org.cn/MulanPSL2
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
 * EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
 * MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
 * See the Mulan PSL v2 for more details.
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

MC_DECL_METATYPE(McLoggerRepository)
