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

#include "McBootConstantGlobal.h"
#include "McBootMacroGlobal.h"

#include "McAbstractQuickBoot.h"

//! 此宏所对应的对象将在Application析构时销毁，所以一旦Application开始析构，就再也不要调用此宏
#define $ (McAbstractQuickBoot::instance()->requestor())

#define MC_QUICKBOOT_SERVICE_EXPORT(FUNC) \
    extern "C" Q_DECL_EXPORT void FUNC() {}

namespace Mc {
namespace QuickBoot {

enum RoutinePriority : int {
    ThreadPool = Mc::Normal - 1, // requestor中线程池优先级
};

}

// 是否添加默认查找路径，默认会到mcservices目录下查找，如果不需要则需要手动调用此函数置为false
MCQUICKBOOT_EXPORT void setDefaultSearch(bool val);
MCQUICKBOOT_EXPORT void setLibraryCheckSymbol(const QLatin1String &symbol);
//! 必须传入全路径，可以是相对与可执行程序的相对路径
MCQUICKBOOT_EXPORT void addServiceSearchPath(const QString &path);
MCQUICKBOOT_EXPORT void addServiceSearchPath(const QStringList &paths);
//! 必须传入动态库全路径，可以是相对与可执行程序的相对路径
MCQUICKBOOT_EXPORT void addServiceLibraryPath(const QString &path);
MCQUICKBOOT_EXPORT void addServiceLibraryPath(const QStringList &paths);

} // namespace Mc

Q_DECLARE_LOGGING_CATEGORY(mcQuickBoot)
