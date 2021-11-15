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
#include "McGlobal.h"

#include <QCoreApplication>
#include <QStandardPaths>
#include <QTimer>
#include <QUrl>

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
#include <corecrt_startup.h>
#define SECNAME ".CRT$XCY"
#pragma section(SECNAME, long, read)
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

MC_GLOBAL_STATIC_BEGIN(coreGlobalStaticData)
QString applicationDirPath;
QString applicationName;
QVector<McMetaType> metaTypes;
MC_GLOBAL_STATIC_END(coreGlobalStaticData)

namespace {

QString getStandardPath(QStandardPaths::StandardLocation type)
{
    auto paths = QStandardPaths::standardLocations(type);
    if (paths.isEmpty()) {
        return QString();
    }
    return paths.first();
}

} // namespace

namespace Mc {

bool waitForExecFunc(const std::function<bool()> &func, qint64 timeout) noexcept
{
    QEventLoop loop;
    QTimer timer;
    timer.setInterval(100);
    QElapsedTimer stopWatcher;
    bool ret = true;

    QObject::connect(&timer, &QTimer::timeout, [&timer, &stopWatcher, &loop, &ret, &func, &timeout]() {
        timer.stop();
        if ((ret = func())) {
            loop.quit();
            return;
        }
        if (timeout != -1 && stopWatcher.elapsed() > timeout) {
            loop.quit();
            return;
        }
        timer.start();
    });

    stopWatcher.start();
    timer.start();
    loop.exec();
    return ret;
}

QString toAbsolutePath(const QString &inPath) noexcept
{
    static QHash<QString, QStandardPaths::StandardLocation> pathPlhs{{"{desktop}", QStandardPaths::DesktopLocation},
                                                                     {"{documents}", QStandardPaths::DocumentsLocation},
                                                                     {"{temp}", QStandardPaths::TempLocation},
                                                                     {"{home}", QStandardPaths::HomeLocation},
                                                                     {"{appLocalData}",
                                                                      QStandardPaths::AppLocalDataLocation},
                                                                     {"{cache}", QStandardPaths::CacheLocation},
                                                                     {"{config}", QStandardPaths::GenericConfigLocation},
                                                                     {"{appData}", QStandardPaths::AppDataLocation}};
    static QStringList pathPlhKeys = pathPlhs.keys();
    auto path = inPath;
    for (const auto &key : pathPlhKeys) {
        const auto &value = pathPlhs.value(key);
        QString plhPath;
        if (path.contains(key) && !(plhPath = getStandardPath(value)).isEmpty()) {
            path.replace(key, plhPath);
        }
    }
    QString dstPath = QDir::toNativeSeparators(path);
    if (QDir::isAbsolutePath(dstPath)) {
        return dstPath;
    }
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

} // namespace Mc

void McMetaType::registerMetaType(const McMetaType &type) noexcept
{
    if (!type.isValid() || type.d->isRegistered.loadRelaxed()) {
        return;
    }
    type.d->isRegistered.storeRelaxed(true);
    type.d->metaType.id();
    type.d->pMetaType.id();
    type.d->sMetaType.id();
    coreGlobalStaticData->metaTypes.append(type);
}

McMetaType McMetaType::fromQMetaType(const QMetaType &type) noexcept
{
    auto itr = std::find_if(coreGlobalStaticData->metaTypes.constBegin(),
                            coreGlobalStaticData->metaTypes.constEnd(),
                            [&type](const McMetaType &t) { return type == t.d->metaType; });
    if (itr == coreGlobalStaticData->metaTypes.constEnd()) {
        return McMetaType();
    }
    return *itr;
}

McMetaType McMetaType::fromPQMetaType(const QMetaType &type) noexcept
{
    auto itr = std::find_if(coreGlobalStaticData->metaTypes.constBegin(),
                            coreGlobalStaticData->metaTypes.constEnd(),
                            [&type](const McMetaType &t) { return type == t.d->pMetaType; });
    if (itr == coreGlobalStaticData->metaTypes.constEnd()) {
        return McMetaType();
    }
    return *itr;
}

McMetaType McMetaType::fromSQMetaType(const QMetaType &type) noexcept
{
    auto itr = std::find_if(coreGlobalStaticData->metaTypes.constBegin(),
                            coreGlobalStaticData->metaTypes.constEnd(),
                            [&type](const McMetaType &t) { return type == t.d->sMetaType; });
    if (itr == coreGlobalStaticData->metaTypes.constEnd()) {
        return McMetaType();
    }
    return *itr;
}

McMetaType McMetaType::fromTypeName(const QByteArray &typeName) noexcept
{
    auto itr = std::find_if(coreGlobalStaticData->metaTypes.constBegin(),
                            coreGlobalStaticData->metaTypes.constEnd(),
                            [&typeName](const McMetaType &t) { return typeName == t.d->metaType.name(); });
    if (itr == coreGlobalStaticData->metaTypes.constEnd()) {
        return McMetaType();
    }
    return *itr;
}

McMetaType McMetaType::fromPTypeName(const QByteArray &typeName) noexcept
{
    auto itr = std::find_if(coreGlobalStaticData->metaTypes.constBegin(),
                            coreGlobalStaticData->metaTypes.constEnd(),
                            [&typeName](const McMetaType &t) { return typeName == t.d->pMetaType.name(); });
    if (itr == coreGlobalStaticData->metaTypes.constEnd()) {
        return McMetaType();
    }
    return *itr;
}

McMetaType McMetaType::fromSTypeName(const QByteArray &typeName) noexcept
{
    auto itr = std::find_if(coreGlobalStaticData->metaTypes.constBegin(),
                            coreGlobalStaticData->metaTypes.constEnd(),
                            [&typeName](const McMetaType &t) { return typeName == t.d->sMetaType.name(); });
    if (itr == coreGlobalStaticData->metaTypes.constEnd()) {
        return McMetaType();
    }
    return *itr;
}

QVector<McMetaType> McMetaType::metaTypes() noexcept
{
    return coreGlobalStaticData->metaTypes;
}

QMetaType McMetaType::metaType() const noexcept
{
    if (!isValid()) {
        return QMetaType();
    }
    return d->metaType;
}

QMetaType McMetaType::pMetaType() const noexcept
{
    if (!isValid()) {
        return QMetaType();
    }
    return d->pMetaType;
}

QMetaType McMetaType::sMetaType() const noexcept
{
    if (!isValid()) {
        return QMetaType();
    }
    return d->sMetaType;
}

void McMetaType::addParentMetaType(const McMetaType &type) const noexcept
{
    if (!isValid() || !type.isValid()) {
        return;
    }
    if (d->parents == nullptr) {
        //! 由于d为编译期静态变量，全局唯一且整个程序生命周期均存在，此处可不析构
        d->parents = new QVector<McMetaType>();
    }
    d->parents->append(type);
}

QVector<McMetaType> McMetaType::parentMetaTypes() const noexcept
{
    if (!isValid() || d->parents == nullptr) {
        return QVector<McMetaType>();
    }
    return *d->parents;
}
