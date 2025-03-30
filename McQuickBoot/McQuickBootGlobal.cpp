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
#include "McQuickBootGlobal.h"

#include <QLibrary>
#include <QThreadPool>

Q_LOGGING_CATEGORY(mcQuickBoot, "mc.quickboot")

MC_GLOBAL_STATIC_BEGIN(staticData)
bool waitThreadPoolDone{true}; //!< 是否等待线程池退出
int threadPoolWaitTimeout{-1}; //!< 等待线程池退出的超时时长，-1为永不超时，直至线程池完全退出
QThreadPool globalThreadPool;

bool isDefaultSearch{true};
QLatin1String libraryCheckSymbol;
QStringList serviceSearchPaths;
QStringList serviceLibraryPaths;
MC_GLOBAL_STATIC_END(staticData)

MC_STATIC2(Mc::RoutineQuickBootServiceLoad)
{
    if (staticData->isDefaultSearch) {
        Mc::addServiceSearchPath(QStringLiteral("./mcservices"));
    }
    Mc::loadLibraryForDir(staticData->serviceSearchPaths);
    Mc::loadLibrary(staticData->serviceLibraryPaths);
}

MC_DESTROY2(Mc::RoutineQuickBootThreadPool)
{
    if (!staticData.exists()) {
        return;
    }
    if (staticData->waitThreadPoolDone) {
        staticData->globalThreadPool.waitForDone(staticData->threadPoolWaitTimeout);
    }
}

namespace Mc {
QThreadPool *globalThreadPool() noexcept
{
    return &staticData->globalThreadPool;
}

void setWaitThreadPoolDone(bool val) noexcept
{
    staticData->waitThreadPoolDone = val;
}

void setThreadPoolWaitTimeout(int val) noexcept
{
    staticData->threadPoolWaitTimeout = val;
}

void setMaxThreadCount(int val) noexcept
{
    staticData->globalThreadPool.setMaxThreadCount(val);
}

void setDefaultSearch(bool val) noexcept
{
    staticData->isDefaultSearch = val;
}

void setLibraryCheckSymbol(QLatin1String symbol) noexcept
{
    staticData->libraryCheckSymbol = symbol;
}

void addServiceSearchPath(const QString &path) noexcept
{
    addServiceSearchPath(QStringList() << path);
}

void addServiceSearchPath(const QStringList &paths) noexcept
{
    staticData->serviceSearchPaths.append(paths);
}

void addServiceLibraryPath(const QString &path) noexcept
{
    addServiceLibraryPath(QStringList() << path);
}

void addServiceLibraryPath(const QStringList &paths) noexcept
{
    staticData->serviceLibraryPaths.append(paths);
}

void loadLibrary(const QString &path) noexcept
{
    loadLibrary(path, staticData->libraryCheckSymbol);
}

void loadLibrary(const QStringList &paths) noexcept
{
    for (auto &tmpPath : paths) {
        loadLibrary(tmpPath);
    }
}

void loadLibraryForDir(const QString &path) noexcept
{
    QDir dir(Mc::toAbsolutePath(path));
    auto fileInfos = dir.entryInfoList(QDir::Files);
    for (auto &fileInfo : qAsConst(fileInfos)) {
        auto path = fileInfo.absoluteFilePath();
        loadLibrary(path);
    }
}

void loadLibraryForDir(const QStringList &paths) noexcept
{
    for (auto &tmpPath : paths) {
        loadLibraryForDir(tmpPath);
    }
}
} // namespace Mc
