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

#include <McCore/Utils/McCancel.h>
#include <McCore/Utils/McPause.h>
#include <McCore/Utils/McProgress.h>

#include "../McQuickBootGlobal.h"

class McAbstractPromise;
namespace McPrivate {
MC_QUICKBOOT_EXPORT void invoke(
    McAbstractPromise *promise, const McSlotObjectWrapper &functor, const QVariantList &arguments) noexcept;
}

MC_FORWARD_DECL_PRIVATE_DATA(McAbstractPromise)

class MC_QUICKBOOT_EXPORT McAbstractPromise : public QObject
{
    Q_OBJECT
public:
    explicit McAbstractPromise(QObject *parent = nullptr) noexcept;
    ~McAbstractPromise() override;

    Q_INVOKABLE void cancel() noexcept;
    Q_INVOKABLE bool isCanceled() const noexcept;
    Q_INVOKABLE void pause() noexcept;
    Q_INVOKABLE void resume() noexcept;
    Q_INVOKABLE bool isPaused() const noexcept;
    Q_INVOKABLE bool isStarted() const noexcept;
    Q_INVOKABLE bool isFinished() const noexcept;
    Q_INVOKABLE QVariant result() const noexcept;
    Q_INVOKABLE bool waitForFinished(qint64 msec = -1) const noexcept;

    bool isAsyncCall() const noexcept;
    void setAsyncCall(bool val) noexcept;

    Q_INVOKABLE void attach(QObject *obj) noexcept;
    Q_INVOKABLE void detach() noexcept;

protected:
    void customEvent(QEvent *event) override;

    virtual void callCallback() noexcept = 0;
    virtual void callError() noexcept = 0;

    QVariant body() const noexcept;
    void setBody(const QVariant &var) noexcept;
    void setStarted(bool val = true) noexcept;
    void setFinished(bool val = true) noexcept;

    McProgress &getProgress() const noexcept;
    McCancel &getCancel() const noexcept;
    McPause &getPause() const noexcept;

private:
    void call() noexcept;

private:
    MC_DECL_PRIVATE(McAbstractPromise)

    friend void McPrivate::invoke(
        McAbstractPromise *promise, const McSlotObjectWrapper &functor, const QVariantList &arguments) noexcept;
};
