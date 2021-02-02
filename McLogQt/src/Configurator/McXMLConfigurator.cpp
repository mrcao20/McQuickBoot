#include "McLog/Configurator/McXMLConfigurator.h"

#include <QCoreApplication>
#include <QDir>
#include <QThread>

#include <McIoc/ApplicationContext/impl/McLocalPathApplicationContext.h>
#include <McIoc/McGlobal.h>

#include "McLog/McLogManager.h"
#include "McLog/Repository/IMcLoggerRepository.h"

IMcApplicationContextPtr McXMLConfigurator::configure(const QString &path,
                                                      const QString &beanName) noexcept
{
    return McXMLConfigurator::configure(QStringList() << path, beanName);
}

IMcApplicationContextPtr McXMLConfigurator::configure(const QStringList &paths,
                                                      const QString &beanName) noexcept
{
    McXMLConfigurator configurator;
    return configurator.doConfigure(paths, beanName);
}

void McXMLConfigurator::configure(IMcApplicationContextConstPtrRef appCtx,
                                  const QString &beanName) noexcept
{
    McXMLConfigurator configurator;
    configurator.doConfigure(appCtx, beanName);
}

IMcApplicationContextPtr McXMLConfigurator::doConfigure(const QStringList &paths,
                                                        const QString &beanName) noexcept
{
    QStringList xmlPaths;
    for(auto path : paths) {
        auto xmlPath = Mc::toAbsolutePath(path);
        xmlPaths << xmlPath;
    }

    McLocalPathApplicationContextPtr appContext = McLocalPathApplicationContextPtr::create(xmlPaths);

    doConfigure(appContext, beanName);

    return appContext;
}

void McXMLConfigurator::doConfigure(IMcApplicationContextConstPtrRef appCtx,
                                    const QString &beanName) noexcept
{
    QThread *thread = new QThread(); //!< 此线程将在LoggerRepository被析构时退出和销毁
    thread->start();

    appCtx->refresh(thread);

    auto rep = appCtx->getBean<IMcLoggerRepository>(beanName);
    McLogManager::instance()->setLoggerRepository(rep);

    McLogManager::installQtMessageHandler();
}
