#include "McLog/Configurator/McXMLConfigurator.h"

#include <QCoreApplication>
#include <QDir>
#include <QThread>

#ifndef MC_NO_IOC
#include <McIoc/ApplicationContext/impl/McLocalPathApplicationContext.h>
#endif

#include "McLog/McLogManager.h"
#include "McLog/Repository/IMcLoggerRepository.h"

McXMLConfigurator::McXMLConfigurator() 
{
}

void McXMLConfigurator::configure(const QString &path, const QString &beanName) noexcept 
{
    McXMLConfigurator configurator;
    configurator.doConfigure(path, beanName);
}

void McXMLConfigurator::doConfigure(const QString &path, const QString &beanName) noexcept 
{
#ifdef MC_NO_IOC
    Q_UNUSED(path)
    Q_UNUSED(beanName)
#else
    auto xmlPath = QDir::toNativeSeparators(path);
    QString sep = ".";
    sep += QDir::separator();
    if(xmlPath.startsWith(sep)) {
        xmlPath.remove(0, 1);
        xmlPath = qApp->applicationDirPath() + xmlPath;
    }
    
    QThread *thread = new QThread();    //!< 此线程将在LoggerRepository被析构时退出和销毁
    McLocalPathApplicationContextPtr appContext = 
            McLocalPathApplicationContextPtr::create(xmlPath);
    appContext->refresh(thread);
    
    auto rep = appContext->getBean<IMcLoggerRepository>(beanName);
    McLogManager::instance()->setLoggerRepository(rep);
    
    thread->start();
#endif
}
