/*
 * Copyright (c) 2021 mrcao20/mrcao20@163.com
 * McQuickBoot is licensed under Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at:
 *          http://license.coscl.org.cn/MulanPSL2
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
 * EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
 * MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
 * See the Mulan PSL v2 for more details.
 */
#include "McXMLConfigurator.h"

#include <QCoreApplication>
#include <QDir>
#include <QThread>

#include <McIoc/ApplicationContext/Impl/McLocalPathApplicationContext.h>

#include "McLogGlobal.h"
#include "McLogManager.h"
#include "Repository/IMcLoggerRepository.h"

IMcApplicationContextPtr McXMLConfigurator::configure(
    const QString &path, const QString &flag, const QString &beanName, QThread *thread) noexcept
{
    return McXMLConfigurator::configure(QStringList() << path, flag, beanName, thread);
}

IMcApplicationContextPtr McXMLConfigurator::configure(
    const QStringList &paths, const QString &flag, const QString &beanName, QThread *thread) noexcept
{
    McXMLConfigurator configurator;
    return configurator.doConfigure(paths, flag, beanName, thread);
}

void McXMLConfigurator::configure(
    const IMcApplicationContextPtr &appCtx, const QString &beanName, QThread *thread) noexcept
{
    McXMLConfigurator configurator;
    configurator.doConfigure(appCtx, beanName, thread);
}

IMcApplicationContextPtr McXMLConfigurator::doConfigure(
    const QStringList &paths, const QString &flag, const QString &beanName, QThread *thread) noexcept
{
    McLocalPathApplicationContextPtr appContext = McLocalPathApplicationContextPtr::create(paths, flag);

    doConfigure(appContext, beanName, thread);

    return appContext;
}

void McXMLConfigurator::doConfigure(
    const IMcApplicationContextPtr &appCtx, const QString &beanName, QThread *thread) noexcept
{
    if (thread == nullptr) {
        thread = new QThread(); //!< 此线程将在LoggerRepository被析构时退出和销毁
        thread->start();
    }

    auto rep = appCtx->getBean<IMcLoggerRepositoryPtr>(beanName, thread);
    McLogManager::instance()->setLoggerRepository(rep);

    McLogManager::installQtMessageHandler();
}
