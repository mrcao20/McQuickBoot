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
#pragma once

#include <QDeadlineTimer>
#include <QDir>
#include <QEvent>
#include <QEventLoop>
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
    template<typename T>
    struct Helper
    {
        static int id() noexcept
        {
            if constexpr (!std::is_enum_v<T> || QtPrivate::IsQEnumHelper<T>::Value) {
                return qMetaTypeId<T>();
            } else {
                return qMetaTypeId<std::underlying_type_t<T>>();
            }
        }
    };
    template<typename T>
    struct Helper<QFlags<T>>
    {
        static int id() noexcept
        {
            if constexpr (QtPrivate::IsQEnumHelper<QFlags<T>>::Value) {
                return qMetaTypeId<QFlags<T>>();
            } else {
                return qMetaTypeId<std::underlying_type_t<T>>();
            }
        }
    };

    static QList<int> metaTypes() noexcept
    {
        QList<int> ms;
        //! 去除安卓上的警告
        Q_UNUSED((ms << ... << Helper<Args>::id()));
        return ms;
    }
#else
    static QList<QMetaType> metaTypes() noexcept
    {
        QList<QMetaType> ms;
        //! 去除安卓上的警告
        Q_UNUSED((ms << ... << QMetaType::fromType<Args>()));
        return ms;
    }
#endif
};

#ifdef MC_USE_QT5
template<typename T>
QVariant toQVariant(const T &data) noexcept
{
    if constexpr (std::is_same_v<char *, T> || std::is_same_v<const char *, T>) {
        return QVariant(data);
    } else if constexpr (!std::is_enum_v<T> || QtPrivate::IsQEnumHelper<T>::Value) {
        return QVariant::fromValue(data);
    } else {
        return static_cast<std::underlying_type_t<T>>(data);
    }
}
template<typename T>
QVariant toQVariant(const QFlags<T> &flags) noexcept
{
    if constexpr (QtPrivate::IsQEnumHelper<QFlags<T>>::Value) {
        return QVariant::fromValue(flags);
    } else {
        typename QFlags<T>::Int intFlags = flags;
        return static_cast<std::underlying_type_t<T>>(intFlags);
    }
}

template<typename T>
struct ToRealValueHelper
{
    static T to(const QVariant &var) noexcept
    {
        if constexpr (!std::is_enum_v<T> || QtPrivate::IsQEnumHelper<T>::Value) {
            return var.value<T>();
        } else {
            return static_cast<T>(var.value<std::underlying_type_t<T>>());
        }
    }
};
template<typename T>
struct ToRealValueHelper<QFlags<T>>
{
    static QFlags<T> to(const QVariant &var) noexcept
    {
        if constexpr (QtPrivate::IsQEnumHelper<QFlags<T>>::Value) {
            return var.value<QFlags<T>>();
        } else {
            return QFlags<T>(static_cast<T>(var.value<std::underlying_type_t<T>>()));
        }
    }
};
template<typename T>
T toRealValue(const QVariant &var) noexcept
{
    return ToRealValueHelper<T>::to(var);
}
#else
template<typename T>
QVariant toQVariant(T &&data) noexcept
{
    return QVariant::fromValue(std::forward<T>(data));
}

template<typename T>
T toRealValue(const QVariant &var) noexcept
{
    return var.value<T>();
}
#endif
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
//! 标识析构代码块的优先级，同一优先级下的代码块没有顺序
enum RoutinePriority : int {
    RoutineMin = std::numeric_limits<int>::min(), //!< 最小值，不可再小
    RoutineQuickBootThreadPool = -200, //!< 启动框架中全局线程池的析构优先级
    RoutineLogDestroy = -100, //!< 日志库析构的优先级
    RoutineNormal = 0, //!< 默认优先级，未特殊指定时均使用此优先级
    RoutineWidgetBuildInTypeRegistry = 10, //!< 界面库内置类型注册的优先级
    RoutineQuickBootServiceLoad = 20, //!< 启动框架的业务库加载优先级
    RoutineMax = std::numeric_limits<int>::max(), //!< 最大值，不可再大
};

enum class NewHandlerType {
    None = -1, //!< 不处理new失败的情况
    Fatal = 0, //!< 如果出现new失败的情况，直接调用qFatal打印错误消息，并退出程序
    Prealloc = 1 //!< 采用预分配内存的方案
};

template<typename Container>
inline bool isContains(int index, const Container &container) noexcept
{
    if (index >= 0 && index < container.size())
        return true;
    return false;
}

/*!
 * \brief waitForExecFunc
 * 执行一个函数，当该函数返回true时或timeout超时时返回。
 * \note 如果当前线程本身处于退出状态，那么此函数会立即返回,
 * 只有当func返回true时当前函数才会返回true，否则都返回false。
 * \param func 将要执行的函数，该函数返回bool值
 * \param timeout 用于超时判断
 * \param flags 事件循环的的执行标志
 * \return 返回函数执行结果
 */
MC_CORE_EXPORT bool waitForExecFunc(
    const std::function<bool()> &func, QDeadlineTimer deadline, QEventLoop::ProcessEventsFlags flags) noexcept;
inline bool waitForExecFunc(const std::function<bool()> &func, QDeadlineTimer deadline) noexcept
{
    return waitForExecFunc(func, deadline, QEventLoop::AllEvents);
}
//! timeout单位为ms
inline bool waitForExecFunc(const std::function<bool()> &func, qint64 timeout) noexcept
{
    return waitForExecFunc(func, QDeadlineTimer(timeout));
}
inline bool waitForExecFunc(const std::function<bool()> &func) noexcept
{
    static constexpr const QDeadlineTimer deadline(QDeadlineTimer::Forever);
    return waitForExecFunc(func, deadline);
}

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
    const QString &path, QLatin1String symbol, QLatin1String checkSymbol) noexcept;
MC_CORE_EXPORT void loadLibrary(const QString &path, QLatin1String checkSymbol) noexcept;
MC_CORE_EXPORT QFunctionPointer loadMemoryLibrary(
    const QByteArray &data, QLatin1String symbol, QLatin1String checkSymbol) noexcept;
MC_CORE_EXPORT void loadMemoryLibrary(const QByteArray &data, QLatin1String checkSymbol) noexcept;
MC_CORE_EXPORT QObject *loadPlugin(
    const QString &pluginPath, const std::function<bool(const QJsonObject &)> &checker = nullptr) noexcept;

//! 如果type为Prealloc，则可以传入第二个参数标识预分配内存的大小，单位：byte，默认64KB。
//! 此时还可以传入第三个参数，为一个函数。当预分配的内存释放之后，就会调用该函数。
//! 否则第二个参数无效
MC_CORE_EXPORT void setNewHandlerType(
    NewHandlerType type, quint64 size = 65536, const std::function<void()> &func = nullptr);

template<typename T>
inline T *loadPlugin(
    const QString &pluginPath, const std::function<bool(const QJsonObject &)> &checker = nullptr) noexcept
{
    QObject *obj = loadPlugin(pluginPath, checker);
    return qobject_cast<T *>(obj);
}

template<typename T>
inline bool qmetaCheck(QObject *obj) noexcept
{
    return (obj != nullptr && obj->metaObject() == &T::staticMetaObject);
}

template<typename T>
inline bool qmetaCheck(const QObjectPtr &obj) noexcept
{
    return (!obj.isNull() && obj->metaObject() == &T::staticMetaObject);
}

template<typename T>
inline bool qmetaInheritsCheck(QObject *obj) noexcept
{
    return (obj != nullptr && obj->metaObject()->inherits(&T::staticMetaObject));
}

template<typename T>
inline bool qmetaInheritsCheck(const QObjectPtr &obj) noexcept
{
    return (!obj.isNull() && obj->metaObject()->inherits(&T::staticMetaObject));
}
} // namespace Mc

namespace McPrivate {
inline constexpr auto extractRoutinePriority(int val = Mc::RoutinePriority::RoutineNormal) noexcept
{
    return val;
}
} // namespace McPrivate
