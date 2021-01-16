#pragma once

#include "../IMcLoggerRepository.h"
#ifndef MC_NO_IOC
#include <McIoc/Thread/IMcDeleteThreadWhenQuit.h>
#endif

MC_FORWARD_DECL_CLASS(IMcAdditionalTask);

MC_FORWARD_DECL_PRIVATE_DATA(McLoggerRepository);

class MCLOGQT_EXPORT McLoggerRepository 
        : public QObject
#ifndef MC_NO_IOC
        , public IMcDeleteThreadWhenQuit
#endif
        , public IMcLoggerRepository 
{
    Q_OBJECT
    MC_DECL_INIT(McLoggerRepository)
#ifndef MC_NO_IOC
    MC_TYPELIST(QObject, IMcLoggerRepository, IMcDeleteThreadWhenQuit)
#else
    MC_TYPELIST(QObject, IMcLoggerRepository)
#endif
    typedef QMap<QString, IMcLoggerPtr> LoggerMap;
    Q_PROPERTY(LoggerMap loggers READ loggers WRITE setLogger)
    Q_PRIVATE_PROPERTY(d, QList<IMcAdditionalTaskPtr> parallelTasks MEMBER parallelTasks)
    Q_PRIVATE_PROPERTY(d, QList<IMcAdditionalTaskPtr> sequentialTasks MEMBER sequentialTasks)
public:
    Q_INVOKABLE McLoggerRepository();
    ~McLoggerRepository() override;
    
    QMap<QString, IMcLoggerPtr> loggers() const noexcept override;
    void addLogger(const QString &loggerName, IMcLoggerConstPtrRef logger) noexcept override;
    void setLogger(const QMap<QString, IMcLoggerPtr> &loggers) noexcept override;
    IMcLoggerPtr getLogger(const QString &loggerName) noexcept override;
    
#ifndef MC_NO_IOC
    void deleteWhenQuit() noexcept override;
#else
    void deleteWhenQuit() noexcept;
#endif

    Q_INVOKABLE
    MC_BEAN_FINISHED
    void finished() noexcept;

private slots:
    void executeTasks() noexcept;

private:
    Q_INVOKABLE void processEvents() noexcept;
    
private:
    MC_DECL_PRIVATE(McLoggerRepository)
};

MC_DECL_METATYPE(McLoggerRepository)
