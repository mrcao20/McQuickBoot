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

#include "McAbstractInvoker.h"

#include <QJSValue>

class McQmlPromise;

class MC_QUICKBOOT_EXPORT McQmlInvoker : public McAbstractInvoker
{
    Q_OBJECT
    MC_FULL_DEFINE(McQmlInvoker, McAbstractInvoker)
    MC_COMPONENT("qmlInvoker")
    MC_POINTER(true)
public:
    explicit McQmlInvoker(QObject *parent = nullptr) noexcept;
    ~McQmlInvoker() override;

    Q_INVOKABLE McQmlPromise *invoke(
        const QString &uri, const QJSValue &data1 = QJSValue(), const QJSValue &data2 = QJSValue()) noexcept;
    Q_INVOKABLE QJSValue syncInvoke(const QString &uri, const QJSValue &data = QJSValue()) noexcept;
};

MC_DECL_METATYPE(McQmlInvoker)
