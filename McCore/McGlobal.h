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
#pragma once

#include <QDir>
#include <QEvent>
#include <QLoggingCategory>
#include <QObject>
#include <QVariant>

#include "McMacroGlobal.h"
#include "McMetaType.h"

Q_DECLARE_LOGGING_CATEGORY(mcCore)

MC_DECL_POINTER(QObject)

namespace McPrivate {
template<typename T>
struct MetaTypeHelper;
template<typename... Args>
struct MetaTypeHelper<QtPrivate::List<Args...>>
{
#ifdef MC_USE_QT5
    static QList<int> metaTypes() noexcept
    {
        QList<int> ms;
        (ms << ... << qMetaTypeId<Args>());
        return ms;
    }
#else
    static QList<QMetaType> metaTypes() noexcept
    {
        QList<QMetaType> ms;
        (ms << ... << QMetaType::fromType<Args>());
        return ms;
    }
#endif
};

template<typename T>
QVariant toQVariant(T &&data) noexcept
{
    return QVariant::fromValue(std::forward<T>(data));
}
template<int N>
QVariant toQVariant(const char (&data)[N]) noexcept
{
    return QVariant(data);
}

namespace LambdaDetail {
template<typename R, typename C, typename M, typename... Args>
struct Types
{
    using IsMutable = M;

    enum { ArgumentCount = sizeof...(Args) };

    using ReturnType = R;
    using Arguments = QtPrivate::List<Args...>;
};
} // namespace LambdaDetail

template<class T>
struct LambdaType : LambdaType<decltype(&T::operator())>
{
};

// 特化版本，函数为非const（对应的lambda表达式为mutable）
template<class R, class C, class... Args>
struct LambdaType<R (C::*)(Args...)> : LambdaDetail::Types<R, C, std::true_type, Args...>
{
};

// 特化版本，函数为const
template<class R, class C, class... Args>
struct LambdaType<R (C::*)(Args...) const> : LambdaDetail::Types<R, C, std::false_type, Args...>
{
};
} // namespace McPrivate

class MC_CORE_EXPORT McCustomEvent : public QEvent
{
    Q_DISABLE_COPY_MOVE(McCustomEvent)
public:
    McCustomEvent(int type, const QVariant &data) noexcept
        : QEvent(static_cast<QEvent::Type>(type))
        , m_data(data)
    {
    }
    ~McCustomEvent() noexcept;

    QVariant data() const noexcept { return m_data; }
    void setData(const QVariant &val) noexcept { m_data = val; }

private:
    QVariant m_data;
};

namespace Mc {
enum RoutinePriority : int {
    Min = -10,
    Normal = 0,
    Max = 10,
};

template<typename Container>
inline bool isContains(int index, const Container &container) noexcept
{
    if (index >= 0 && index < container.size())
        return true;
    return false;
}

/*!
 * \brief waitForExecFunc，执行一个函数，当该函数返回true时或timeout超时时返回
 *
 * 注意：如果当前线程本身处于退出状态，那么此函数会立即返回
 * 如果timeout设置为-1，则表示永不超时
 * 只有当func返回true时当前函数才会返回true，否则都返回false
 * \param func 将要执行的函数，该函数返回bool值
 * \param timeout 超时时长，单位: ms
 * \return 返回函数执行结果
 */
MC_CORE_EXPORT bool waitForExecFunc(const std::function<bool()> &func, qint64 timeout = -1) noexcept;

MC_CORE_EXPORT void registerPathPlaceholder(const QString &placeholder, const std::function<QString()> &func) noexcept;

/*!
 * \brief 将传入的路径转换为绝对路径，支持占位符，自带的占位符及其对应关系如下:
 * {desktop} = QStandardPaths::DesktopLocation
 * {documents} = QStandardPaths::DocumentsLocation
 * {temp} = QStandardPaths::TempLocation
 * {home} = QStandardPaths::HomeLocation
 * {data}/{appLocalData} = QStandardPaths::AppLocalDataLocation第一个路径
 * {cache} = QStandardPaths::CacheLocation
 * {config} = QStandardPaths::GenericConfigLocation第一个路径
 * {appData} = QStandardPaths::AppDataLocation第一个路径
 *
 * 支持自定义占位符，参考registerPathPlaceholder
 *
 * \see registerPathPlaceholder
 */
MC_CORE_EXPORT QString toAbsolutePath(const QString &path) noexcept;
MC_CORE_EXPORT QString applicationDirPath() noexcept;
MC_CORE_EXPORT QDir applicationDir() noexcept;
MC_CORE_EXPORT void setApplicationDirPath(const QString &val) noexcept;
MC_CORE_EXPORT QString applicationFilePath() noexcept;
MC_CORE_EXPORT void setApplicationFilePath(const QString &val) noexcept;

using StartUpFunction = std::function<void()>;
using CleanUpFunction = std::function<void()>;
MC_CORE_EXPORT void addPreRoutine(int priority, const StartUpFunction &func) noexcept;
MC_CORE_EXPORT void callPreRoutine() noexcept;
MC_CORE_EXPORT void cleanPreRoutine() noexcept;
MC_CORE_EXPORT void addPostRoutine(int priority, const CleanUpFunction &func) noexcept;

MC_CORE_EXPORT QFunctionPointer loadLibrary(
    const QString &path, const QLatin1String &symbol, const QLatin1String &checkSymbol) noexcept;
MC_CORE_EXPORT void loadLibrary(const QString &path, const QLatin1String &checkSymbol) noexcept;
MC_CORE_EXPORT QFunctionPointer loadMemoryLibrary(
    const QByteArray &data, const QLatin1String &symbol, const QLatin1String &checkSymbol) noexcept;
MC_CORE_EXPORT void loadMemoryLibrary(const QByteArray &data, const QLatin1String &checkSymbol) noexcept;
MC_CORE_EXPORT QObject *loadPlugin(
    const QString &pluginPath, const std::function<bool(const QJsonObject &)> &checker = nullptr) noexcept;

template<typename T>
T *loadPlugin(const QString &pluginPath, const std::function<bool(const QJsonObject &)> &checker = nullptr) noexcept
{
    QObject *obj = loadPlugin(pluginPath, checker);
    return qobject_cast<T *>(obj);
}
} // namespace Mc

namespace McPrivate {
inline constexpr auto extractRoutinePriority(int val = Mc::RoutinePriority::Normal) noexcept
{
    return val;
}
} // namespace McPrivate
