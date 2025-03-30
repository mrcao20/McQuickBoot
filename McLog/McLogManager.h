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

#include "McLogMacroGlobal.h"

MC_FORWARD_DECL_CLASS(IMcLoggerRepository)

MC_FORWARD_DECL_PRIVATE_DATA(McLogManager)

class MC_LOG_EXPORT McLogManager
{
    MC_DECL_INIT(McLogManager)
    Q_DISABLE_COPY_MOVE(McLogManager)
    McLogManager() noexcept;

public:
    ~McLogManager();

    IMcLoggerRepositoryPtr loggerRepository() const noexcept;
    void setLoggerRepository(const IMcLoggerRepositoryPtr &val) noexcept;

    static McLogManager *instance() noexcept;
    static void installQtMessageHandler() noexcept;
    static void uninstallQtMessageHandler() noexcept;
    static void runTask() noexcept;
    static void handlerWhenQuit(bool val) noexcept;

private:
    static void customMessageHandler(
        QtMsgType msgType, const QMessageLogContext &msgLogCtx, const QString &msg) noexcept;

    void output(QtMsgType msgType, const QMessageLogContext &msgLogCtx, const QString &msg) noexcept;

private:
    MC_DECL_PRIVATE(McLogManager)
};
