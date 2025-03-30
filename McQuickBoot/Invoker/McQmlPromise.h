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

#include "McAbstractPromise.h"

class QJSValue;

MC_FORWARD_DECL_PRIVATE_DATA(McQmlPromise)

class MC_QUICKBOOT_EXPORT McQmlPromise : public McAbstractPromise
{
    Q_OBJECT
    MC_DECL_SUPER(McAbstractPromise)
public:
    explicit McQmlPromise(QObject *parent = nullptr) noexcept;
    ~McQmlPromise() override;

    Q_INVOKABLE McQmlPromise *then(const QJSValue &callback) noexcept;
    Q_INVOKABLE McQmlPromise *syncThen(const QJSValue &callback) noexcept;
    Q_INVOKABLE McQmlPromise *asyncThen(const QJSValue &callback) noexcept;
    Q_INVOKABLE McQmlPromise *onCanceled(const QJSValue &callback) noexcept;
    Q_INVOKABLE McQmlPromise *onError(const QJSValue &callback) noexcept;
    Q_INVOKABLE McQmlPromise *onCurrentProgress(const QJSValue &callback) noexcept;
    Q_INVOKABLE McQmlPromise *onTotalProgress(const QJSValue &callback) noexcept;

protected:
    void callCallback() noexcept override;
    void callCanceled() noexcept override;
    void callError() noexcept override;

private:
    void call(QJSValue &callback) noexcept;

private:
    MC_DECL_PRIVATE(McQmlPromise)
};

#ifdef MC_USE_QT5
Q_DECLARE_METATYPE(McQmlPromise *)
#endif
