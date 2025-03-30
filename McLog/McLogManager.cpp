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
#include "McLogManager.h"

#include "Logger/IMcLogger.h"
#include "Repository/IMcLoggerRepository.h"

static void customMessageHandlerNone(QtMsgType msgType, const QMessageLogContext &msgLogCtx, const QString &msg) noexcept
{
    Q_UNUSED(msgType)
    Q_UNUSED(msgLogCtx)
    Q_UNUSED(msg)
}

MC_DECL_PRIVATE_DATA(McLogManager)
IMcLoggerRepositoryPtr loggerRepository;
bool handlerWhenQuit{false};
MC_DECL_PRIVATE_DATA_END

MC_INIT(McLogManager)
MC_DESTROY(Mc::RoutinePriority::RoutineLogDestroy)
if (!McLogManager::instance()->loggerRepository().isNull()) {
    McLogManager::instance()->loggerRepository()->flushWhenQuit();
}
McLogManager::uninstallQtMessageHandler();
if (McLogManager::instance()->d->handlerWhenQuit) {
    qInstallMessageHandler(customMessageHandlerNone);
}
McLogManager::instance()->setLoggerRepository(IMcLoggerRepositoryPtr());
MC_INIT_END

McLogManager::McLogManager() noexcept
{
    MC_NEW_PRIVATE_DATA(McLogManager);
}

McLogManager::~McLogManager() {}

IMcLoggerRepositoryPtr McLogManager::loggerRepository() const noexcept
{
    return d->loggerRepository;
}

void McLogManager::setLoggerRepository(const IMcLoggerRepositoryPtr &val) noexcept
{
    d->loggerRepository = val;
}

McLogManager *McLogManager::instance() noexcept
{
    static McLogManager instance;
    return &instance;
}

void McLogManager::installQtMessageHandler() noexcept
{
    qInstallMessageHandler(customMessageHandler);
}

void McLogManager::uninstallQtMessageHandler() noexcept
{
    qInstallMessageHandler(nullptr);
}

void McLogManager::runTask() noexcept
{
    instance()->loggerRepository()->runTask();
}

void McLogManager::handlerWhenQuit(bool val) noexcept
{
    instance()->d->handlerWhenQuit = val;
}

void McLogManager::customMessageHandler(
    QtMsgType msgType, const QMessageLogContext &msgLogCtx, const QString &msg) noexcept
{
    McLogManager::instance()->output(msgType, msgLogCtx, msg);
}

void McLogManager::output(QtMsgType msgType, const QMessageLogContext &msgLogCtx, const QString &msg) noexcept
{
    auto rep = loggerRepository();
    if (rep.isNull()) {
        return;
    }
    auto logger = rep->getLogger(msgLogCtx.category);
    if (logger.isNull()) {
        return;
    }
    logger->log(msgType, msgLogCtx, msg);
}
