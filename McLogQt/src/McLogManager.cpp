#include "McLog/McLogManager.h"

#include "McLog/Repository/IMcLoggerRepository.h"
#include "McLog/Logger/IMcLogger.h"

MC_DECL_PRIVATE_DATA(McLogManager)
IMcLoggerRepositoryPtr loggerRepository;
MC_DECL_PRIVATE_DATA_END

McLogManager::McLogManager() 
{
    MC_NEW_PRIVATE_DATA(McLogManager);
}

McLogManager::~McLogManager() 
{
    McLogManager::uninstallQtMessageHandler();
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
    //! 好像有bug，存在如果主程序中没有静态局部对象或者没有优先调用一次此函数时析构不掉
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
