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
#include "McLog/McLogManager.h"

#include "McLog/Repository/IMcLoggerRepository.h"
#include "McLog/Logger/IMcLogger.h"

MC_DECL_PRIVATE_DATA(McLogManager)
IMcLoggerRepositoryPtr loggerRepository;
MC_DECL_PRIVATE_DATA_END

MC_INIT(McLogManager)
MC_DESTROY(Mc::RoutinePriority::Min)
McLogManager::instance()->d->loggerRepository->flushWhenQuit();
McLogManager::uninstallQtMessageHandler();  //!< 此函数内部会加锁，loggerRepository析构时一定不会进入output函数
McLogManager::instance()->d->loggerRepository.reset();
MC_INIT_END

McLogManager::McLogManager() 
{
    MC_NEW_PRIVATE_DATA(McLogManager);
}

McLogManager::~McLogManager() 
{
}

IMcLoggerRepositoryPtr McLogManager::loggerRepository() const noexcept 
{
    return d->loggerRepository;
}

void McLogManager::setLoggerRepository(IMcLoggerRepositoryConstPtrRef val) noexcept 
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
    instance()->d->loggerRepository->runTask();
}

void McLogManager::customMessageHandler(QtMsgType msgType, const QMessageLogContext &msgLogCtx, const QString &msg) noexcept 
{
    McLogManager::instance()->output(msgType, msgLogCtx, msg);
}

void McLogManager::output(QtMsgType msgType, const QMessageLogContext &msgLogCtx, const QString &msg) noexcept 
{
    auto rep = loggerRepository();
    if(rep.isNull()) {
        return;
    }
    auto logger = rep->getLogger(msgLogCtx.category);
    if(logger.isNull()) {
        return;
    }
    logger->log(msgType, msgLogCtx, msg);
}
