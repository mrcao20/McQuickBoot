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

#include <QObject>
#include <QVariant>

#include "../McQuickBootGlobal.h"

class McAbstractPromise;
class IMcApplicationContext;

MC_FORWARD_DECL_CLASS(McControllerContainer)

MC_FORWARD_DECL_PRIVATE_DATA(McAbstractInvoker)

class MC_QUICKBOOT_EXPORT McAbstractInvoker : public QObject
{
    Q_OBJECT
    MC_PRIVATE_PROPERTY(McControllerContainerPtr, controllerContainer, MEMBER controllerContainer)
public:
    explicit McAbstractInvoker(QObject *parent = nullptr) noexcept;
    ~McAbstractInvoker() override;

    IMcApplicationContext *appCtx() const noexcept;
    void setAppCtx(IMcApplicationContext *val) noexcept;

    Q_INVOKABLE QObject *getBean(const QString &name) const noexcept;
    //    Q_INVOKABLE QObject *getModel(const QString &name) const noexcept;

protected:
    void run(McAbstractPromise *promise, const QString &uri, const QVariantList &data, bool async = true) noexcept;
    QVariant getBeanToVariant(const QString &name) const noexcept;

private:
    MC_DECL_PRIVATE(McAbstractInvoker)
};

MC_DECL_METATYPE(McAbstractInvoker)
