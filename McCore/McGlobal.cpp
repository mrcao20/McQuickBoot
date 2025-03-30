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
#include "McGlobal.h"

#include <QCoreApplication>
#include <QElapsedTimer>
#include <QPluginLoader>
#include <QScopeGuard>
#include <QStandardPaths>
#include <QTimer>
#include <QUrl>

#include "MemoryLibrary/McMemoryLibrary.h"

Q_LOGGING_CATEGORY(mcCore, "mc.core")

namespace McPrivate {
using StartUpFuncs = QMap<int, QVector<Mc::StartUpFunction>>;
Q_GLOBAL_STATIC(StartUpFuncs, preFuncs)
using CleanUpFuncs = QMap<int, QVector<Mc::CleanUpFunction>>;
Q_GLOBAL_STATIC(CleanUpFuncs, postFuncs)

int coreStaticInit()
{
    qAddPreRoutine([]() { Mc::callPreRoutine(); });
    qAddPostRoutine([]() {
        CleanUpFuncs funcs;
        funcs.swap(*postFuncs);
        auto keys = funcs.keys();
        for (int i = keys.length() - 1; i >= 0; --i) {
            auto list = funcs.value(keys.value(i));
            for (int j = 0; j < list.length(); ++j) {
                list.at(j)();
            }
        }
        Q_QGS_postFuncs::guard.storeRelaxed(QtGlobalStatic::Initialized);
    });
    return 1;
}
#ifdef Q_CC_GNU
static void __attribute__((constructor)) __coreStaticInit__(void)
{
    coreStaticInit();
}
#elif Q_CC_MSVC
# include <corecrt_startup.h>
# define SECNAME ".CRT$XCY"
# pragma section(SECNAME, long, read)
int __coreStaticInit__()
{
    coreStaticInit();
    return 0;
}
__declspec(allocate(SECNAME)) _PIFV dummy[] = {__coreStaticInit__};
#else
Q_CONSTRUCTOR_FUNCTION(coreStaticInit)
#endif
} // namespace McPrivate

McCustomEvent::~McCustomEvent() noexcept {}

static QString getStandardPath(QStandardPaths::StandardLocation type)
{
    auto paths = QStandardPaths::standardLocations(type);
    if (paths.isEmpty()) {
        return QString();
    }
    return paths.first();
}

static QSharedPointer<QLibrary> loadLibraryHelper(const QString &path, QLatin1String checkSymbol) noexcept
{
    auto libPath = Mc::toAbsolutePath(path);
    QFileInfo fileInfo(libPath);
    if (!fileInfo.suffix().isEmpty() && !QLibrary::isLibrary(libPath)) {
        return QSharedPointer<QLibrary>();
    }
    auto library = QSharedPointer<QLibrary>::create(libPath);
    if (!library->load()) {
        qCWarning(mcCore(), "%s cannot load!! error string: %s", qUtf8Printable(libPath),
            qUtf8Printable(library->errorString()));
        return QSharedPointer<QLibrary>();
    }
    auto cleanup = qScopeGuard([]() { Mc::callPreRoutine(); });
    if (checkSymbol.isNull() || checkSymbol.isEmpty()) {
        return library;
    }
    auto func = library->resolve(checkSymbol.data());
    if (func != nullptr) {
        return library;
    }
    cleanup.dismiss();
    Mc::cleanPreRoutine();
    library->unload();
    return QSharedPointer<QLibrary>();
}

static McMemoryLibrary loadMemoryLibraryHelper(const QByteArray &data, QLatin1String checkSymbol) noexcept
{
    McMemoryLibrary library(data);
    if (!library.load()) {
        return McMemoryLibrary();
    }
    auto cleanup = qScopeGuard([]() { Mc::callPreRoutine(); });
    if (checkSymbol.isNull() || checkSymbol.isEmpty()) {
        return library;
    }
    auto func = library.resolve(checkSymbol);
    if (func != nullptr) {
        return library;
    }
    cleanup.dismiss();
    Mc::cleanPreRoutine();
    library.unload();
    return McMemoryLibrary();
}

MC_GLOBAL_STATIC_BEGIN(coreGlobalStaticData)
QString applicationDirPath;
QString applicationName;
QHash<QString, std::function<QString()>> pathPlaceholders{
    {QStringLiteral("{desktop}"), std::bind(getStandardPath, QStandardPaths::DesktopLocation)},
    {QStringLiteral("{documents}"), std::bind(getStandardPath, QStandardPaths::DocumentsLocation)},
    {QStringLiteral("{temp}"), std::bind(getStandardPath, QStandardPaths::TempLocation)},
    {QStringLiteral("{home}"), std::bind(getStandardPath, QStandardPaths::HomeLocation)},
    {QStringLiteral("{appLocalData}"), std::bind(getStandardPath, QStandardPaths::AppLocalDataLocation)},
    {QStringLiteral("{data}"), std::bind(getStandardPath, QStandardPaths::AppLocalDataLocation)},
    {QStringLiteral("{cache}"), std::bind(getStandardPath, QStandardPaths::CacheLocation)},
    {QStringLiteral("{config}"), std::bind(getStandardPath, QStandardPaths::GenericConfigLocation)},
    {QStringLiteral("{appData}"), std::bind(getStandardPath, QStandardPaths::AppDataLocation)},
};

char *preallocMemory{nullptr};
std::function<void()> newHandlerFunc;
MC_GLOBAL_STATIC_END(coreGlobalStaticData)

static void fatalNewHandler()
{
    Mc::setNewHandlerType(Mc::NewHandlerType::None);
    qFatal("out of memory");
}

static void preallocNewHandler()
{
    Mc::setNewHandlerType(Mc::NewHandlerType::None);
    MC_SAFETY_DELETE2(coreGlobalStaticData->preallocMemory)
    if (coreGlobalStaticData->newHandlerFunc != nullptr) {
        coreGlobalStaticData->newHandlerFunc();
    }
}

namespace Mc {
bool waitForExecFunc(
    const std::function<bool()> &func, QDeadlineTimer deadline, QEventLoop::ProcessEventsFlags flags) noexcept
{
    QEventLoop loop;
    QTimer timer;
    timer.setSingleShot(true);
    using namespace std::chrono_literals;
    timer.setInterval(100ms);
    bool ret = true;

    timer.callOnTimeout([&timer, &loop, &ret, func, deadline]() {
        if ((ret = func())) {
            loop.quit();
            return;
        }
        if (deadline.hasExpired()) {
            loop.quit();
            return;
        }
        timer.start();
    });

    timer.start();
    loop.exec(flags);
    return ret;
}

void registerPathPlaceholder(const QString &placeholder, const std::function<QString()> &func) noexcept
{
    coreGlobalStaticData->pathPlaceholders.insert(placeholder, func);
}

QString toAbsolutePath(const QString &inPath) noexcept
{
    QStringList pathPlhKeys = coreGlobalStaticData->pathPlaceholders.keys();
    auto path = inPath;
    for (const auto &key : pathPlhKeys) {
        const auto &value = coreGlobalStaticData->pathPlaceholders.value(key);
        QString plhPath;
        if (path.contains(key) && !(plhPath = value()).isEmpty()) {
            path.replace(key, plhPath);
        }
    }
    if (QDir::isAbsolutePath(path)) {
        return path;
    }
    QString dstPath = QDir::toNativeSeparators(path);
    QString sepDot = ".";
    QString sepDotDot = "..";
    sepDot += QDir::separator();
    sepDotDot += QDir::separator();
    if (dstPath.startsWith(sepDot) || dstPath.startsWith(sepDotDot)) {
        dstPath = Mc::applicationDirPath() + QDir::separator() + dstPath;
    } else {
        QUrl url(path);
        if (url.isLocalFile()) {
            dstPath = url.toLocalFile();
            dstPath = QDir::toNativeSeparators(dstPath);
            if (!QDir::isAbsolutePath(dstPath)) {
                dstPath = Mc::applicationDirPath() + QDir::separator() + dstPath;
            }
            url = QUrl::fromLocalFile(dstPath);
        }
        dstPath = url.toString();
    }
    dstPath = QDir::cleanPath(dstPath);
    return dstPath;
}

QString applicationDirPath() noexcept
{
    if (coreGlobalStaticData->applicationDirPath.isEmpty()) {
        return QCoreApplication::applicationDirPath();
    }
    return coreGlobalStaticData->applicationDirPath;
}

QDir applicationDir() noexcept
{
    return QDir(applicationDirPath());
}

void setApplicationDirPath(const QString &val) noexcept
{
    coreGlobalStaticData->applicationDirPath = val;
}

QString applicationFilePath() noexcept
{
    if (coreGlobalStaticData->applicationName.isEmpty()) {
        return QCoreApplication::applicationFilePath();
    }
    return applicationDir().absoluteFilePath(coreGlobalStaticData->applicationName);
}

void setApplicationFilePath(const QString &val) noexcept
{
    QFileInfo fileInfo(val);
    coreGlobalStaticData->applicationDirPath = fileInfo.absolutePath();
    coreGlobalStaticData->applicationName = fileInfo.fileName();
}

void addPreRoutine(int priority, const StartUpFunction &func) noexcept
{
    McPrivate::StartUpFuncs *funcs = McPrivate::preFuncs;
    if (!funcs) {
        return;
    }
    (*funcs)[priority].prepend(func);
}

void callPreRoutine() noexcept
{
    using namespace McPrivate;
    StartUpFuncs funcs;
    funcs.swap(*preFuncs);
    auto keys = funcs.keys();
    for (int i = keys.length() - 1; i >= 0; --i) {
        auto list = funcs.value(keys.value(i));
        for (int j = 0; j < list.length(); ++j) {
            list.at(j)();
        }
    }
    Q_QGS_preFuncs::guard.storeRelaxed(QtGlobalStatic::Initialized);
}

void cleanPreRoutine() noexcept
{
    McPrivate::preFuncs->clear();
}

void addPostRoutine(int priority, const CleanUpFunction &func) noexcept
{
    McPrivate::CleanUpFuncs *funcs = McPrivate::postFuncs;
    if (!funcs) {
        return;
    }
    (*funcs)[priority].prepend(func);
}

QFunctionPointer loadLibrary(const QString &path, QLatin1String symbol, QLatin1String checkSymbol) noexcept
{
    auto library = loadLibraryHelper(path, checkSymbol);
    if (library.isNull()) {
        return nullptr;
    }
    return library->resolve(symbol.data());
}

void loadLibrary(const QString &path, QLatin1String checkSymbol) noexcept
{
    loadLibraryHelper(path, checkSymbol);
}

QFunctionPointer loadMemoryLibrary(const QByteArray &data, QLatin1String symbol, QLatin1String checkSymbol) noexcept
{
    auto library = loadMemoryLibraryHelper(data, checkSymbol);
    if (!library.isLoaded()) {
        return nullptr;
    }
    return library.resolve(symbol);
}

void loadMemoryLibrary(const QByteArray &data, QLatin1String checkSymbol) noexcept
{
    loadMemoryLibraryHelper(data, checkSymbol);
}

QObject *loadPlugin(const QString &pluginPath, const std::function<bool(const QJsonObject &)> &checker) noexcept
{
    if (pluginPath.isEmpty()) {
        return nullptr;
    }
    auto path = Mc::toAbsolutePath(pluginPath);
    QPluginLoader loader(path);
    if (checker != nullptr && !checker(loader.metaData())) {
        return nullptr;
    }
    if (!loader.load()) {
        qCWarning(
            mcCore(), "%s cannot load!! error string: %s", qUtf8Printable(path), qUtf8Printable(loader.errorString()));
        return nullptr;
    }
    Mc::callPreRoutine();
    return loader.instance();
}

void setNewHandlerType(NewHandlerType type, quint64 size, const std::function<void()> &func)
{
    if (type == NewHandlerType::None) {
        std::set_new_handler(nullptr);
    } else if (type == NewHandlerType::Fatal) {
        std::set_new_handler(&fatalNewHandler);
    } else {
        if (size < 0) {
            return;
        }
        coreGlobalStaticData->newHandlerFunc = func;
        if (size > 0) {
            coreGlobalStaticData->preallocMemory = new char[size];
        }
        std::set_new_handler(&preallocNewHandler);
    }
}
} // namespace Mc
