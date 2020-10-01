#include "McLog/Configurator/McXMLConfigurator.h"

#include <QCoreApplication>
#include <QDir>
#include <QThread>

#ifndef MC_NO_IOC
#include <McIoc/ApplicationContext/impl/McLocalPathApplicationContext.h>
#include <McIoc/McGlobal.h>
#endif

#include "McLog/McLogManager.h"
#include "McLog/Repository/IMcLoggerRepository.h"

McXMLConfigurator::McXMLConfigurator() 
{
}

void McXMLConfigurator::configure(const QString &path, const QString &beanName) noexcept 
{
    McXMLConfigurator::configure(QStringList() << path, beanName);
}

void McXMLConfigurator::configure(const QStringList &paths, const QString &beanName) noexcept
{
    McXMLConfigurator configurator;
    configurator.doConfigure(paths, beanName);
}

void McXMLConfigurator::doConfigure(const QStringList &paths, const QString &beanName) noexcept 
{
#ifdef MC_NO_IOC
    Q_UNUSED(path)
    Q_UNUSED(beanName)
#else
    QStringList xmlPaths;
    for(auto path : paths) {
        auto xmlPath = Mc::toAbsolutePath(path);
        xmlPaths << xmlPath;
    }
    
    QThread *thread = new QThread();    //!< 此线程将在LoggerRepository被析构时退出和销毁
    McLocalPathApplicationContextPtr appContext = 
            McLocalPathApplicationContextPtr::create(xmlPaths);
    appContext->refresh(thread);
    
    auto rep = appContext->getBean<IMcLoggerRepository>(beanName);
    McLogManager::instance()->setLoggerRepository(rep);
    
    thread->start();
#endif
}
