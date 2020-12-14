#include "McLog/McLogManager.h"

#include "McLog/Repository/IMcLoggerRepository.h"
#include "McLog/Logger/IMcLogger.h"

MC_DECL_PRIVATE_DATA(McLogManager)
IMcLoggerRepositoryPtr loggerRepository;
MC_DECL_PRIVATE_DATA_END

MC_INIT(McLogManager)
MC_DESTROY(Mc::RoutinePriority::Min)
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
