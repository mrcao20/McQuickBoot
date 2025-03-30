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

#include <McIoc/McIocGlobal.h>

#include "McQuickBootConstantGlobal.h"
#include "McQuickBootMacroGlobal.h"

Q_DECLARE_LOGGING_CATEGORY(mcQuickBoot)

class QThreadPool;

// clang-format off
#define MC_QUICKBOOT_SERVICE_EXPORT(FUNC) \
    extern "C" Q_DECL_EXPORT void FUNC() {}
// clang-format on

namespace Mc {
MC_QUICKBOOT_EXPORT QThreadPool *globalThreadPool() noexcept;
MC_QUICKBOOT_EXPORT void setWaitThreadPoolDone(bool val) noexcept;
MC_QUICKBOOT_EXPORT void setThreadPoolWaitTimeout(int val) noexcept;
MC_QUICKBOOT_EXPORT void setMaxThreadCount(int val) noexcept;

// 是否添加默认查找路径，默认会到mcservices目录下查找，如果不需要则需要手动调用此函数置为false
MC_QUICKBOOT_EXPORT void setDefaultSearch(bool val) noexcept;
MC_QUICKBOOT_EXPORT void setLibraryCheckSymbol(QLatin1String symbol) noexcept;
//! 必须传入全路径，可以是相对与可执行程序的相对路径
MC_QUICKBOOT_EXPORT void addServiceSearchPath(const QString &path) noexcept;
MC_QUICKBOOT_EXPORT void addServiceSearchPath(const QStringList &paths) noexcept;
//! 必须传入动态库全路径，可以是相对与可执行程序的相对路径
MC_QUICKBOOT_EXPORT void addServiceLibraryPath(const QString &path) noexcept;
MC_QUICKBOOT_EXPORT void addServiceLibraryPath(const QStringList &paths) noexcept;
//! 加载动态库，会自动处理库中静态代码块
MC_QUICKBOOT_EXPORT void loadLibrary(const QString &path) noexcept;
MC_QUICKBOOT_EXPORT void loadLibrary(const QStringList &paths) noexcept;
MC_QUICKBOOT_EXPORT void loadLibraryForDir(const QString &path) noexcept;
MC_QUICKBOOT_EXPORT void loadLibraryForDir(const QStringList &paths) noexcept;
} // namespace Mc
