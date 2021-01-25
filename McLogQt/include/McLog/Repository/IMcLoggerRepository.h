#pragma once

#include "../McLogGlobal.h"

MC_FORWARD_DECL_CLASS(IMcLogger)

class IMcLoggerRepository 
{
public:
    virtual ~IMcLoggerRepository() = default;
    
    virtual QMap<QString, IMcLoggerPtr> loggers() const noexcept = 0;
    virtual void addLogger(const QString &loggerName, IMcLoggerConstPtrRef logger) noexcept = 0;
    virtual void setLogger(const QMap<QString, IMcLoggerPtr> &loggers) noexcept = 0;
    virtual IMcLoggerPtr getLogger(const QString &loggerName) noexcept = 0;
    virtual void runTask() noexcept = 0;
};

MC_DECL_METATYPE(IMcLoggerRepository)
