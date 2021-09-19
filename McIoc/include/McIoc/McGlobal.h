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

#include <functional>
#include <mutex>

#include <QDir>
#include <QEvent>
#include <QLoggingCategory>
#include <QObject>
#include <QSharedPointer>
#include <QtCore/qmutex.h>

#include "BeanFactory/McBeanGlobal.h"
#include "McConstantGlobal.h"
#include "McMacroGlobal.h"
#include "McVersion.h"

Q_DECLARE_LOGGING_CATEGORY(mcIoc)

MC_FORWARD_DECL_CLASS(IMcApplicationContext)

template<>
inline bool qMapLessThanKey(const QVariant &key1, const QVariant &key2)
{
    return qMapLessThanKey(key1.data(), key2.data());
}

namespace McPrivate {

template<typename T>
struct IsQVariantHelper
{
    enum { Value = true, Value2 = false };
};
template<>
struct IsQVariantHelper<const char *>
{
    enum { Value = false, Value2 = true };
};
template<>
struct IsQVariantHelper<char *>
{
    enum { Value = false, Value2 = true };
};
template<int N>
struct IsQVariantHelper<char[N]>
{
    enum { Value = false, Value2 = true };
};

template<typename T>
struct QVariantSelector
{
    enum { Value = false };
};
template<>
struct QVariantSelector<QVariant>
{
    enum { Value = true };
};
template<>
struct QVariantSelector<const QVariant>
{
    enum { Value = true };
};
template<>
struct QVariantSelector<QVariant &>
{
    enum { Value = true };
};
template<>
struct QVariantSelector<const QVariant &>
{
    enum { Value = true };
};
template<>
struct QVariantSelector<QtPrivate::List<>>
{
    enum { Value = false };
};
template<typename... Args>
struct QVariantSelector<QtPrivate::List<Args...>>
{
    enum { Value = QVariantSelector<typename QtPrivate::List<Args...>::Car>::Value };
};

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
{};

// 特化版本，函数为非const（对应的lambda表达式为mutable）
template<class R, class C, class... Args>
struct LambdaType<R (C::*)(Args...)> : LambdaDetail::Types<R, C, std::true_type, Args...>
{};

// 特化版本，函数为const
template<class R, class C, class... Args>
struct LambdaType<R (C::*)(Args...) const> : LambdaDetail::Types<R, C, std::false_type, Args...>
{};

template <typename Mutex, typename Lock =
#if defined(__cpp_guaranteed_copy_elision) && __cpp_guaranteed_copy_elision >= 201606L
# if defined(__cpp_lib_scoped_lock) && __cpp_lib_scoped_lock >= 201703L
          std::scoped_lock
# else
          std::lock_guard
# endif
#else
          std::unique_lock
#endif
          <typename std::decay<Mutex>::type>
>
Lock mc_scoped_lock(Mutex &mutex)
{
    return Lock(mutex);
}

template <typename Mutex, typename Lock = std::unique_lock<typename std::decay<Mutex>::type>>
Lock mc_unique_lock(Mutex &mutex)
{
    return Lock(mutex);
}

template <typename Mutex, typename Lock = std::unique_lock<typename std::decay<Mutex>::type>>
Lock mc_unique_lock(Mutex *mutex)
{
    return mutex ? Lock(*mutex) : Lock() ;
}

//! beanName应当用QByteArray
QString getBeanName(const QMetaObject *metaObj) noexcept;

} // namespace McPrivate

MC_DECL_POINTER(QObject)

class MCIOC_EXPORT McCustomEvent : public QEvent {
public:
    McCustomEvent(int type, const QVariant &data) noexcept
        : QEvent(static_cast<QEvent::Type>(type))
        , m_data(data)
    {}
    ~McCustomEvent() noexcept;
    
    QVariant data() const noexcept {
        return m_data;
    }
    void setData(const QVariant &val) noexcept {
        m_data = val;
    }
    
private:
    QVariant m_data;
};

namespace Mc {

enum RoutinePriority : int {
    Min = -10,
    Normal = 0,
    Max = 10
};

#ifdef MC_MANUAL_ENABLE_IOC
namespace Ioc {
MCIOC_EXPORT void init() noexcept;
}
#endif

template<typename Container>
bool isContains(int index, const Container &container) noexcept
{
    if (index >= 0 && index < container.size())
        return true;
    return false;
}

/*!
 * \brief mcWaitForExecFunc，执行一个函数，当该函数返回true时或timeout超时时返回
 * 
 * 注意：如果当前线程本身处于退出状态，那么此函数会立即返回
 * 如果timeout设置为-1，则表示永不超时
 * 只有当func返回true时当前函数才会返回true，否则都返回false
 * \param func 将要执行的函数，该函数返回bool值
 * \param timeout 超时时长，单位: ms
 * \return 返回函数执行结果
 */
MCIOC_EXPORT bool waitForExecFunc(
        const std::function<bool()> &func
        , qint64 timeout = -1) noexcept;

/*!
 * \brief mcToAbsolutePath
 * 
 * 如果path为file:///开头的相对路径，那么转为绝对路径。其他格式不变
 * \param path
 * \return 
 */
MCIOC_EXPORT QString toAbsolutePath(const QString &path) noexcept;
MCIOC_EXPORT QString applicationDirPath() noexcept;
MCIOC_EXPORT QDir applicationDir() noexcept;
MCIOC_EXPORT void setApplicationDirPath(const QString &val) noexcept;
MCIOC_EXPORT QString applicationFilePath() noexcept;
MCIOC_EXPORT void setApplicationFilePath(const QString &val) noexcept;

//! 获取所有被Component标记的bean
MCIOC_EXPORT QList<QString> getAllComponent(IMcApplicationContextConstPtrRef appCtx) noexcept;
//! 获取所有组建类型为componentType的bean的名称
MCIOC_EXPORT QList<QString> getComponents(IMcApplicationContextConstPtrRef appCtx, const QString &componentType) noexcept;
//! 是否被Component标记
MCIOC_EXPORT bool isComponent(const QMetaObject *metaObj) noexcept;
//! 传入的元对象的组件类型是否为type
MCIOC_EXPORT bool isComponentType(const QMetaObject *metaObj, const QString &type) noexcept;
MCIOC_EXPORT bool isContainedTag(const QString &tags, const QString &tag) noexcept;
MCIOC_EXPORT QObject *getObject(IMcApplicationContext *appCtx, const QString &beanName) noexcept;

using StartUpFunction = std::function<void()>;
using CleanUpFunction = std::function<void()>;
MCIOC_EXPORT void addPreRoutine(int priority, const StartUpFunction &func) noexcept;
MCIOC_EXPORT void callPreRoutine() noexcept;
MCIOC_EXPORT void cleanPreRoutine() noexcept;
MCIOC_EXPORT void addPostRoutine(int priority, const CleanUpFunction &func) noexcept;

namespace Ioc {

MCIOC_EXPORT void connect(const QString &beanName,
                          const QString &sender,
                          const QString &signal,
                          const QString &receiver,
                          const QString &slot,
                          Qt::ConnectionType type = Qt::AutoConnection) noexcept;

MCIOC_EXPORT void connect(const QMetaObject *metaObj,
                          const QString &sender,
                          const QString &signal,
                          const QString &receiver,
                          const QString &slot,
                          Qt::ConnectionType type = Qt::AutoConnection) noexcept;

MCIOC_EXPORT QMetaObject::Connection connect(IMcApplicationContext *appCtx,
                                             const QString &sender,
                                             const QString &signal,
                                             const QString &receiver,
                                             const QString &slot,
                                             Qt::ConnectionType type = Qt::AutoConnection) noexcept;

MCIOC_EXPORT QMetaObject::Connection connect(IMcApplicationContext *appCtx,
                                             const QString &sender,
                                             const QString &signal,
                                             QObject *receiver,
                                             const QString &slot,
                                             Qt::ConnectionType type = Qt::AutoConnection) noexcept;

MCIOC_EXPORT bool disconnect(IMcApplicationContext *appCtx,
                             const QString &sender,
                             const QString &signal,
                             const QString &receiver,
                             const QString &slot) noexcept;

MCIOC_EXPORT bool disconnect(IMcApplicationContext *appCtx,
                             const QString &sender,
                             const QString &signal,
                             QObject *receiver,
                             const QString &slot) noexcept;
} // namespace Ioc
} // namespace Mc
