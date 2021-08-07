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
#include "McLog/Configurator/McXMLConfigurator.h"

#include <QCoreApplication>
#include <QDir>
#include <QThread>

#include <McIoc/ApplicationContext/impl/McLocalPathApplicationContext.h>
#include <McIoc/McGlobal.h>

#include "McLog/McLogManager.h"
#include "McLog/Repository/IMcLoggerRepository.h"

IMcApplicationContextPtr McXMLConfigurator::configure(const QString &path,
                                                      const QString &flag,
                                                      const QString &beanName) noexcept
{
    return McXMLConfigurator::configure(QStringList() << path, flag, beanName);
}

IMcApplicationContextPtr McXMLConfigurator::configure(const QStringList &paths,
                                                      const QString &flag,
                                                      const QString &beanName) noexcept
{
    McXMLConfigurator configurator;
    return configurator.doConfigure(paths, flag, beanName);
}

void McXMLConfigurator::configure(IMcApplicationContextConstPtrRef appCtx,
                                  const QString &beanName) noexcept
{
    McXMLConfigurator configurator;
    configurator.doConfigure(appCtx, beanName);
}

IMcApplicationContextPtr McXMLConfigurator::doConfigure(const QStringList &paths,
                                                        const QString &flag,
                                                        const QString &beanName) noexcept
{
    QStringList xmlPaths;
    for(auto path : paths) {
        auto xmlPath = Mc::toAbsolutePath(path);
        xmlPaths << xmlPath;
    }

    McLocalPathApplicationContextPtr appContext = McLocalPathApplicationContextPtr::create(xmlPaths,
                                                                                           flag);

    doConfigure(appContext, beanName);

    return appContext;
}

void McXMLConfigurator::doConfigure(IMcApplicationContextConstPtrRef appCtx,
                                    const QString &beanName) noexcept
{
    QThread *thread = new QThread(); //!< 此线程将在LoggerRepository被析构时退出和销毁
    thread->start();

    auto rep = appCtx->getBean<IMcLoggerRepository>(beanName, thread);
    McLogManager::instance()->setLoggerRepository(rep);

    McLogManager::installQtMessageHandler();
}
