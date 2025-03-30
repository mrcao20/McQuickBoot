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

#include "../McQuickBootGlobal.h"

MC_FORWARD_DECL_CLASS(McAbstractQuickBoot)
MC_FORWARD_DECL_CLASS(IMcApplicationContext)

MC_FORWARD_DECL_PRIVATE_DATA(McAbstractQuickBoot)

class MC_QUICKBOOT_EXPORT McAbstractQuickBoot : public QObject
{
    Q_OBJECT
    MC_DECL_INIT(McAbstractQuickBoot)
public:
    explicit McAbstractQuickBoot(QObject *parent = nullptr) noexcept;
    ~McAbstractQuickBoot() override;

    static McAbstractQuickBootPtr instance() noexcept;

    IMcApplicationContextPtr applicationContext() const noexcept;

protected:
    static void setInstance(const McAbstractQuickBootPtr &ins) noexcept;

    void loadContext(const QStringList &preloadBeans = QStringList()) noexcept;

    virtual void loadInvoker(IMcApplicationContext *appCtx, QThread *workThread) noexcept = 0;
    virtual void flushInvoker(IMcApplicationContext *appCtx) noexcept = 0;

private:
    MC_DECL_PRIVATE(McAbstractQuickBoot)
};
