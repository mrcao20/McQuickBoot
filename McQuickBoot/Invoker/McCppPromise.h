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

MC_FORWARD_DECL_PRIVATE_DATA(McCppPromise)

class MC_QUICKBOOT_EXPORT McCppPromise : public McAbstractPromise
{
    Q_OBJECT
    MC_DECL_SUPER(McAbstractPromise)
    enum class CallbackType {
        Sync,
        Async,
        Error,
    };

public:
    explicit McCppPromise(QObject *parent = nullptr) noexcept;
    ~McCppPromise() override;

    using super::result;
    template<typename T>
    T result() const noexcept
    {
        return body().value<T>();
    }

    QPointer<McCppPromise> capture() noexcept;

    template<typename Func>
    McCppPromise &then(const typename QtPrivate::FunctionPointer<Func>::Object *receiver, Func callback) noexcept
    {
        setCallback(CallbackType::Sync, McSlotObjectWrapper::build(receiver, callback));
        return *this;
    }
    template<typename Func>
    McCppPromise &then(Func callback) noexcept
    {
        setCallback(CallbackType::Sync, McSlotObjectWrapper::build(nullptr, callback));
        return *this;
    }

    template<typename Func>
    McCppPromise &syncThen(const typename QtPrivate::FunctionPointer<Func>::Object *receiver, Func callback) noexcept
    {
        setCallback(CallbackType::Sync, McSlotObjectWrapper::build(receiver, callback));
        return *this;
    }
    template<typename Func>
    McCppPromise &syncThen(Func callback) noexcept
    {
        setCallback(CallbackType::Sync, McSlotObjectWrapper::build(nullptr, callback));
        return *this;
    }

    template<typename Func>
    McCppPromise &asyncThen(const typename QtPrivate::FunctionPointer<Func>::Object *receiver, Func callback) noexcept
    {
        setCallback(CallbackType::Async, McSlotObjectWrapper::build(receiver, callback));
        return *this;
    }
    template<typename Func>
    McCppPromise &asyncThen(Func callback) noexcept
    {
        setCallback(CallbackType::Async, McSlotObjectWrapper::build(nullptr, callback));
        return *this;
    }

    template<typename Func>
    McCppPromise &error(const typename QtPrivate::FunctionPointer<Func>::Object *receiver, Func callback) noexcept
    {
        setCallback(CallbackType::Error, McSlotObjectWrapper::build(receiver, callback));
        return *this;
    }
    template<typename Func>
    McCppPromise &error(Func callback) noexcept
    {
        setCallback(CallbackType::Error, McSlotObjectWrapper::build(nullptr, callback));
        return *this;
    }

    template<typename Func>
    McCppPromise &currentProgress(
        const typename QtPrivate::FunctionPointer<Func>::Object *recever, Func callback) noexcept
    {
        getProgress().currentCallback(recever, callback);
        return *this;
    }
    template<typename Func>
    McCppPromise &currentProgress(Func callback) noexcept
    {
        getProgress().currentCallback(callback);
        return *this;
    }
    template<typename Func>
    McCppPromise &totalProgress(const typename QtPrivate::FunctionPointer<Func>::Object *recever, Func callback) noexcept
    {
        getProgress().totalCallback(recever, callback);
        return *this;
    }
    template<typename Func>
    McCppPromise &totalProgress(Func callback) noexcept
    {
        getProgress().totalCallback(callback);
        return *this;
    }

protected:
    void callCallback() noexcept override;
    void callError() noexcept override;

private:
    void setCallback(CallbackType type, const McSlotObjectWrapper &functor) noexcept;

private:
    MC_DECL_PRIVATE(McCppPromise)
};
