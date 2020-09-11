#include "McLog/Repository/impl/McLoggerRepository.h"

#include "McLog/Logger/IMcLogger.h"

MC_DECL_PRIVATE_DATA(McLoggerRepository)
QMap<QString, IMcLoggerPtr> loggers;
MC_DECL_PRIVATE_DATA_END

MC_INIT(McLoggerRepository)
MC_REGISTER_BEAN_FACTORY(MC_TYPELIST(McLoggerRepository))
MC_REGISTER_MAP_CONVERTER(LoggerMap)
MC_INIT_END

McLoggerRepository::McLoggerRepository() 
{
    MC_NEW_PRIVATE_DATA(McLoggerRepository);
}

McLoggerRepository::~McLoggerRepository() 
{
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
