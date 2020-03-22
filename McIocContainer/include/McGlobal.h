#pragma once

#include <functional>

#include <QSharedPointer>
#include <QObject>

#include "BeanFactory/McBeanGlobal.h"

using QObjectPtr = QSharedPointer<QObject>;
using QObjectConstPtrRef = const QSharedPointer<QObject> &;

MC_BEGIN_NAMESPACE

/*!
 * \brief mcWaitForExecFunc，执行一个函数，当该函数返回true时或timeout超时时返回
 * 
 * 注意：如果当前线程本身处于退出状态，那么此函数会立即返回
 * 如果timeout设置为-1，则表示用不超时
 * 只有当func返回true时当前函数才会返回true，否则都返回false
 * \param func 将要执行的函数，该函数返回bool值
 * \param timeout 超时时长，单位: ms
 * \return 返回函数执行结果
 */
MCIOCCONTAINER_EXPORT bool mcWaitForExecFunc(
        const std::function<bool()> &func
        , qint64 timeout = -1) noexcept;

MC_END_NAMESPACE
