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

#include <QLoggingCategory>

#include <McIoc/McGlobal.h>
#include <McIoc/Utils/Event/McEventDispatcher.h>

#include "McBootConstantGlobal.h"
#include "McBootMacroGlobal.h"

#include "McAbstractQuickBoot.h"

//! 此宏所对应的对象将在Application析构时销毁，所以一旦Application开始析构，就再也不要调用此宏
#define $ (McAbstractQuickBoot::instance()->requestor())

#define MC_QUICKBOOT_SERVICE_EXPORT(FUNC) \
    extern "C" Q_DECL_EXPORT void FUNC() {}

namespace Mc {
namespace QuickBoot {

enum RoutinePriority {
    ThreadPool = Mc::Normal - 1, //!< requestor中线程池优先级
};

enum class NewHandlerType {
    None = -1, //!< 不处理new失败的情况
    Fatal = 0, //!< 如果出现new失败的情况，直接调用qFatal打印错误消息，并退出程序
    Prealloc = 1 //!< 采用预分配内存的方案
};

} // namespace QuickBoot

// 是否添加默认查找路径，默认会到mcservices目录下查找，如果不需要则需要手动调用此函数置为false
MCQUICKBOOT_EXPORT void setDefaultSearch(bool val);
MCQUICKBOOT_EXPORT void setLibraryCheckSymbol(const QLatin1String &symbol);
//! 必须传入全路径，可以是相对与可执行程序的相对路径
MCQUICKBOOT_EXPORT void addServiceSearchPath(const QString &path);
MCQUICKBOOT_EXPORT void addServiceSearchPath(const QStringList &paths);
//! 必须传入动态库全路径，可以是相对与可执行程序的相对路径
MCQUICKBOOT_EXPORT void addServiceLibraryPath(const QString &path);
MCQUICKBOOT_EXPORT void addServiceLibraryPath(const QStringList &paths);
//! 加载动态库，会自动处理库中静态代码块
MCQUICKBOOT_EXPORT void loadLibrary(const QString &path);
MCQUICKBOOT_EXPORT void loadLibrary(const QStringList &paths);
MCQUICKBOOT_EXPORT void loadLibraryForDir(const QString &path);
MCQUICKBOOT_EXPORT void loadLibraryForDir(const QStringList &paths);

//! 如果type为Prealloc，则可以传入第二个参数标识预分配内存的大小，单位：byte，默认64KB。
//! 此时还可以传入第三个参数，为一个函数。当预分配的内存释放之后，就会调用该函数。
//! 否则第二个参数无效
MCQUICKBOOT_EXPORT void setNewHandlerType(QuickBoot::NewHandlerType type,
                                          quint64 size = 65536,
                                          const std::function<void()> &func = nullptr);

} // namespace Mc

Q_DECLARE_LOGGING_CATEGORY(mcQuickBoot)
