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
#include <QScxmlStateMachine>

namespace {

struct McBootGlobalStaticData
{
    bool isDefaultSearch{true};
    QLatin1String libraryCheckSymbol;
    QStringList serviceSearchPaths;
    QStringList serviceLibraryPaths;
};

} // namespace

MC_GLOBAL_STATIC(McBootGlobalStaticData, mcBootGlobalStaticData)

MC_STATIC(Mc::RoutinePriority::Max + 9)
qRegisterMetaType<QScxmlStateMachine *>();
if (mcBootGlobalStaticData->isDefaultSearch) {
    Mc::addServiceSearchPath("./mcservices");
}
for (auto &searchPath : qAsConst(mcBootGlobalStaticData->serviceSearchPaths)) {
    QDir dir(Mc::toAbsolutePath(searchPath));
    auto fileInfos = dir.entryInfoList(QDir::Files);
    for (auto &fileInfo : qAsConst(fileInfos)) {
        auto path = fileInfo.absoluteFilePath();
        Mc::addServiceLibraryPath(path);
    }
}
for (auto &tmpPath : qAsConst(mcBootGlobalStaticData->serviceLibraryPaths)) {
    auto libPath = Mc::toAbsolutePath(tmpPath);
    if (!QLibrary::isLibrary(libPath)) {
        continue;
    }
    QLibrary library(libPath);
    if (!library.load()) {
        qWarning() << libPath << "cannot load!! error string:" << library.errorString();
        continue;
    }
    auto cleanup = qScopeGuard([]() { Mc::callPreRoutine(); });
    if (mcBootGlobalStaticData->libraryCheckSymbol.isNull()
        || mcBootGlobalStaticData->libraryCheckSymbol.isEmpty()) {
        continue;
    }
    auto func = library.resolve(mcBootGlobalStaticData->libraryCheckSymbol.data());
    if (func != nullptr) {
        continue;
    }
    cleanup.dismiss();
    Mc::cleanPreRoutine();
    library.unload();
}
MC_STATIC_END

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

} // namespace Mc

Q_LOGGING_CATEGORY(mcQuickBoot, "mc.quickboot")
