#include "McLog/Configurator/McSettingConfigurator.h"

#include <QSettings>
#include <QThread>
#include <QDebug>

#include "McLog/McLogManager.h"
#include "McLog/Repository/impl/McLoggerRepository.h"
#include "McLog/Logger/impl/McLogger.h"
#include "McLog/Appender/impl/McConsoleAppender.h"
#include "McLog/Appender/impl/McSizeRollingFileAppender.h"

McSettingConfigurator::McSettingConfigurator() 
{
}

void McSettingConfigurator::configure(QSettings &settings) noexcept 
{
    McSettingConfigurator configurator;
    QThread *thread = new QThread();
    configurator.moveToThread(thread);
    configurator.doConfigure(settings);
    thread->start();

    McLogManager::installQtMessageHandler();
}

void McSettingConfigurator::doConfigure(QSettings &settings) noexcept 
{
    settings.value("logger");   //!< 在beginGroup之前不预先获取一次之后就读不到数据
    settings.beginGroup("logger");
    
    McLoggerRepositoryPtr rep = McLoggerRepositoryPtr::create();
    
    auto loggers = settings.childGroups();
    for(auto logger : loggers) {
        settings.beginGroup(logger);
        
        auto l = configLogger(settings);
        if(!l.isNull()) {
            rep->addLogger(logger, l);
        }
        
        settings.endGroup();
    }
    
    rep->moveToThread(thread());
    rep->deleteWhenQuit();
    McLogManager::instance()->setLoggerRepository(rep);
    
    settings.endGroup();
}

IMcLoggerPtr McSettingConfigurator::configLogger(QSettings &settings) noexcept 
{
    McLoggerPtr logger = McLoggerPtr::create();
    
    auto threshold = settings.value("threshold", "debug-").toString();
    logger->setThreshold(threshold);
    
    settings.beginGroup("appender");
    logger->setAppenders(configAppenders(settings));
    settings.endGroup();
    
    logger->finished();
    logger->moveToThread(thread());
    return std::move(logger);
}

QList<IMcConfigurableAppenderPtr> McSettingConfigurator::configAppenders(QSettings &settings) noexcept 
{
    QList<IMcConfigurableAppenderPtr> appenders;
    
    auto list = settings.childGroups();
    for(auto appenderName : list) {
        settings.beginGroup(appenderName);
        
        if(appenderName == "file") {
            McSizeRollingFileAppenderPtr appender = McSizeRollingFileAppenderPtr::create();
            
            QString threshold;
            auto value = settings.value("threshold", "");
            if(value.type() == QVariant::String) {
                threshold = value.toString();
            }else{
                threshold = value.toStringList().join(',');
            }
            appender->setThreshold(threshold);
            appender->setImmediateFlush(settings.value("immediateFlush", false).toBool());
            appender->setMaxFileSize(settings.value("maxFileSize", "10MB").toString());
            appender->setBackupDirPath(settings.value("backupDirPath", "").toString());
            appender->setBackupDirPattern(settings.value("backupDirPattern", "").toString());
            appender->setDirPath(settings.value("dirPath", "").toString());
            appender->setFileNamePattern(settings.value("fileNamePattern").toString());

            appender->finished();
            appender->moveToThread(thread());
            appender->threadFinished();

            appenders.append(appender);
        }else if(appenderName == "console") {
            McConsoleAppenderPtr appender = McConsoleAppenderPtr::create();
            
            QString threshold;
            auto value = settings.value("threshold", "");
            if(value.type() == QVariant::String) {
                threshold = value.toString();
            }else{
                threshold = value.toStringList().join(',');
            }
            appender->setThreshold(threshold);
            appender->setImmediateFlush(settings.value("immediateFlush", false).toBool());

            appender->finished();
            appender->moveToThread(thread());
            appender->threadFinished();

            appenders.append(appender);
        }
        
        settings.endGroup();
    }
    
    return appenders;
}
