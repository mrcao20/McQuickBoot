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
for (auto searchPath : mcBootGlobalStaticData->serviceSearchPaths) {
    QDir dir(searchPath);
    auto fileInfos = dir.entryInfoList(QDir::Files);
    for (auto fileInfo : fileInfos) {
        auto path = fileInfo.absoluteFilePath();
        Mc::addServiceLibraryPath(path);
    }
}
for (auto libPath : mcBootGlobalStaticData->serviceLibraryPaths) {
    if (!QLibrary::isLibrary(libPath)) {
        continue;
    }
    QLibrary library(libPath);
    if (!library.load()) {
        qWarning() << libPath << "cannot load!!";
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
    for (auto path : paths) {
        mcBootGlobalStaticData->serviceSearchPaths.append(Mc::toAbsolutePath(path));
    }
}

void addServiceLibraryPath(const QString &path)
{
    addServiceLibraryPath(QStringList() << path);
}

void addServiceLibraryPath(const QStringList &paths)
{
    for (auto path : paths) {
        mcBootGlobalStaticData->serviceLibraryPaths.append(Mc::toAbsolutePath(path));
    }
}

} // namespace Mc

Q_LOGGING_CATEGORY(mcQuickBoot, "mc.quickboot")
