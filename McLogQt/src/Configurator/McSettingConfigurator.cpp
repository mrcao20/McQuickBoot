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
