#pragma once

#include "../IMcLoggerRepository.h"
#include "../../Utils/McDeleteThreadWhenQuit.h"

MC_FORWARD_DECL_PRIVATE_DATA(McLoggerRepository);

class McLoggerRepository 
        : public QObject
        , public McDeleteThreadWhenQuit
        , public IMcLoggerRepository 
{
    
    Q_OBJECT
    MC_DECL_INIT(McLoggerRepository)
    MC_DEFINE_TYPELIST(QObject, MC_DECL_TYPELIST(IMcLoggerRepository), MC_DECL_TYPELIST(McDeleteThreadWhenQuit))
    typedef QMap<QString, IMcLoggerPtr> LoggerMap;
    Q_PROPERTY(LoggerMap loggers READ loggers WRITE setLogger)
public:
    Q_INVOKABLE McLoggerRepository();
    ~McLoggerRepository() override;
    
    QMap<QString, IMcLoggerPtr> loggers() const noexcept override;
    void addLogger(const QString &loggerName, IMcLoggerConstPtrRef logger) noexcept override;
    void setLogger(const QMap<QString, IMcLoggerPtr> &loggers) noexcept override;
    IMcLoggerPtr getLogger(const QString &loggerName) noexcept override;
    
private:
    MC_DECL_PRIVATE(McLoggerRepository)
};

MC_DECL_METATYPE(McLoggerRepository)
