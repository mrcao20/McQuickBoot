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

#include <optional>

#include "../McQuickBootGlobal.h"
#include "McControllerEntity.h"

MC_FORWARD_DECL_CLASS(IMcApplicationContext)

MC_FORWARD_DECL_PRIVATE_DATA(McControllerContainer)

class MC_QUICKBOOT_EXPORT McControllerContainer : public QObject
{
    Q_OBJECT
    MC_DECL_INIT(McControllerContainer)
    MC_COMPONENT("controllerContainer")
public:
    explicit McControllerContainer(QObject *parent = nullptr) noexcept;
    ~McControllerContainer();

    void init(const IMcApplicationContextPtr &appCtx) noexcept;

    std::optional<McControllerEntity> find(const QString &uri) const noexcept;
    std::optional<McControllerEntity> find(const QString &uri, const QVariantList &data) const noexcept;
    std::optional<McControllerEntity> find(const QString &uri, const QVariantMap &data) const noexcept;

private:
    bool splitBeanAndFunc(const QString &uri, QString &beanName, QString &funcName) const noexcept;
#ifdef MC_USE_QT5
    QVariant maybeBuildCallback(const QVariant &originVar, int metaType) const noexcept;
#else
    QVariant maybeBuildCallback(const QVariant &originVar, const QMetaType &metaType) const noexcept;
#endif

private:
    MC_DECL_PRIVATE(McControllerContainer)
};

MC_DECL_METATYPE(McControllerContainer)
