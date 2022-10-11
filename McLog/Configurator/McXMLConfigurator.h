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

#include "../McLogMacroGlobal.h"

class QThread;

MC_FORWARD_DECL_CLASS(IMcApplicationContext)

class MC_LOG_EXPORT McXMLConfigurator
{
public:
    static IMcApplicationContextPtr configure(const QString &path, const QString &flag = QString(),
        const QString &beanName = "defaultLoggerRepository", QThread *thread = nullptr) noexcept;
    static IMcApplicationContextPtr configure(const QStringList &paths, const QString &flag = QString(),
        const QString &beanName = "defaultLoggerRepository", QThread *thread = nullptr) noexcept;
    static void configure(const IMcApplicationContextPtr &appCtx, const QString &beanName = "defaultLoggerRepository",
        QThread *thread = nullptr) noexcept;

private:
    IMcApplicationContextPtr doConfigure(
        const QStringList &paths, const QString &flag, const QString &beanName, QThread *thread = nullptr) noexcept;
    void doConfigure(
        const IMcApplicationContextPtr &appCtx, const QString &beanName, QThread *thread = nullptr) noexcept;
};
