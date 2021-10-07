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
#include "McBoot/McBootGlobal.h"

#include <QDebug>
#include <QDir>
#include <QLibrary>
#include <QScopeGuard>

namespace {

struct McBootGlobalStaticData
{
    bool isDefaultSearch{true};
    QLatin1String libraryCheckSymbol;
    QStringList serviceSearchPaths;
    QStringList serviceLibraryPaths;
    char *preallocMemory{nullptr};
    std::function<void()> newHandlerFunc;
};

} // namespace

MC_GLOBAL_STATIC(McBootGlobalStaticData, mcBootGlobalStaticData)

MC_STATIC(Mc::RoutinePriority::Max + 9)
if (mcBootGlobalStaticData->isDefaultSearch) {
    Mc::addServiceSearchPath("./mcservices");
}
Mc::loadLibraryForDir(mcBootGlobalStaticData->serviceSearchPaths);
Mc::loadLibrary(mcBootGlobalStaticData->serviceLibraryPaths);
MC_STATIC_END

namespace {

void fatalNewHandler()
{
    Mc::setNewHandlerType(Mc::QuickBoot::NewHandlerType::None);
    qFatal("out of memory");
}

void preallocNewHandler()
{
    Mc::setNewHandlerType(Mc::QuickBoot::NewHandlerType::None);
    MC_SAFETY_DELETE2(mcBootGlobalStaticData->preallocMemory)
    if (mcBootGlobalStaticData->newHandlerFunc != nullptr) {
        mcBootGlobalStaticData->newHandlerFunc();
    }
}

} // namespace

namespace Mc {

void setDefaultSearch(bool val)
{
    mcBootGlobalStaticData->isDefaultSearch = val;
}

void setLibraryCheckSymbol(const QLatin1String &symbol)
{
    mcBootGlobalStaticData->libraryCheckSymbol = symbol;
}

void addServiceSearchPath(const QString &path)
{
    addServiceSearchPath(QStringList() << path);
}

void addServiceSearchPath(const QStringList &paths)
{
    for (auto &path : qAsConst(paths)) {
        mcBootGlobalStaticData->serviceSearchPaths.append(path);
    }
}

void addServiceLibraryPath(const QString &path)
{
    addServiceLibraryPath(QStringList() << path);
}

void addServiceLibraryPath(const QStringList &paths)
{
    for (auto &path : qAsConst(paths)) {
        mcBootGlobalStaticData->serviceLibraryPaths.append(path);
    }
}

void loadLibrary(const QString &path)
{
    auto libPath = Mc::toAbsolutePath(path);
    if (!QLibrary::isLibrary(libPath)) {
        return;
    }
    QLibrary library(libPath);
    if (!library.load()) {
        qCWarning(mcQuickBoot) << libPath << "cannot load!! error string:" << library.errorString();
        return;
    }
    auto cleanup = qScopeGuard([]() { Mc::callPreRoutine(); });
    if (mcBootGlobalStaticData->libraryCheckSymbol.isNull()
        || mcBootGlobalStaticData->libraryCheckSymbol.isEmpty()) {
        return;
    }
    auto func = library.resolve(mcBootGlobalStaticData->libraryCheckSymbol.data());
    if (func != nullptr) {
        return;
    }
    cleanup.dismiss();
    Mc::cleanPreRoutine();
    library.unload();
}

void loadLibrary(const QStringList &paths)
{
    for (auto &tmpPath : paths) {
        loadLibrary(tmpPath);
    }
}

void loadLibraryForDir(const QString &path)
{
    QDir dir(Mc::toAbsolutePath(path));
    auto fileInfos = dir.entryInfoList(QDir::Files);
    for (auto &fileInfo : qAsConst(fileInfos)) {
        auto path = fileInfo.absoluteFilePath();
        loadLibrary(path);
    }
}

void loadLibraryForDir(const QStringList &paths)
{
    for (auto &tmpPath : paths) {
        loadLibraryForDir(tmpPath);
    }
}

void setNewHandlerType(QuickBoot::NewHandlerType type,
                       quint64 size,
                       const std::function<void()> &func)
{
    if (type == QuickBoot::NewHandlerType::None) {
        std::set_new_handler(nullptr);
    } else if (type == QuickBoot::NewHandlerType::Fatal) {
        std::set_new_handler(&fatalNewHandler);
    } else {
        if (size < 0) {
            return;
        }
        mcBootGlobalStaticData->newHandlerFunc = func;
        if (size > 0) {
            mcBootGlobalStaticData->preallocMemory = new char[size];
        }
        std::set_new_handler(&preallocNewHandler);
    }
}

} // namespace Mc

Q_LOGGING_CATEGORY(mcQuickBoot, "mc.quickboot")
