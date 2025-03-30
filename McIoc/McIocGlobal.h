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

#include <QLoggingCategory>

#include <McCore/McGlobal.h>

#include "McIocMacroGlobal.h"

Q_DECLARE_LOGGING_CATEGORY(mcIoc)

MC_FORWARD_DECL_CLASS(IMcApplicationContext)

namespace McPrivate {
QString getBeanName(const QMetaObject *metaObj) noexcept;
} // namespace McPrivate

namespace Mc {
//! 获取所有被Component标记的bean
MC_IOC_EXPORT QList<QString> getAllComponent(const IMcApplicationContextPtr &appCtx) noexcept;
//! 获取所有组件类型为componentType的bean的名称
MC_IOC_EXPORT QList<QString> getComponents(
    const IMcApplicationContextPtr &appCtx, const QString &componentType) noexcept;
//! 是否被Component标记
MC_IOC_EXPORT bool isComponent(const QMetaObject *metaObj) noexcept;
//! 传入的元对象的组件类型是否为type
MC_IOC_EXPORT bool isComponentType(const QMetaObject *metaObj, const QString &type) noexcept;
MC_IOC_EXPORT bool isContainedTag(const QByteArray &tags, const QByteArray &tag) noexcept;
MC_IOC_EXPORT QObject *getObject(IMcApplicationContext *appCtx, const QString &beanName) noexcept;

namespace Ioc {
MC_IOC_EXPORT void connect(const QString &beanName, const QString &sender, const QString &signal,
    const QString &receiver, const QString &slot, Qt::ConnectionType type = Qt::AutoConnection) noexcept;

MC_IOC_EXPORT void connect(const QMetaObject *metaObj, const QString &sender, const QString &signal,
    const QString &receiver, const QString &slot, Qt::ConnectionType type = Qt::AutoConnection) noexcept;

MC_IOC_EXPORT QMetaObject::Connection connect(IMcApplicationContext *appCtx, const QString &sender,
    const QString &signal, const QString &receiver, const QString &slot,
    Qt::ConnectionType type = Qt::AutoConnection) noexcept;

MC_IOC_EXPORT QMetaObject::Connection connect(IMcApplicationContext *appCtx, const QString &sender,
    const QString &signal, QObject *receiver, const QString &slot,
    Qt::ConnectionType type = Qt::AutoConnection) noexcept;

MC_IOC_EXPORT bool disconnect(IMcApplicationContext *appCtx, const QString &sender, const QString &signal,
    const QString &receiver, const QString &slot) noexcept;

MC_IOC_EXPORT bool disconnect(IMcApplicationContext *appCtx, const QString &sender, const QString &signal,
    QObject *receiver, const QString &slot) noexcept;
} // namespace Ioc
} // namespace Mc
