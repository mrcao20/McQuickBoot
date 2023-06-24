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
        Canceled,
        Error,
    };

public:
    explicit McCppPromise(QObject *parent = nullptr) noexcept;
    ~McCppPromise() override;

    using super::result;
    template<typename T>
    T result() const noexcept
    {
        return McPrivate::toRealValue<T>(body());
    }

    QPointer<McCppPromise> capture() noexcept;

    template<typename Func>
    McCppPromise &then(const typename QtPrivate::FunctionPointer<Func>::Object *receiver, Func callback) noexcept
    {
        static_assert(int(QtPrivate::FunctionPointer<Func>::ArgumentCount) <= 1,
            "The number of parameters of callback function can only be less than or equal to 1");

        setCallback(CallbackType::Sync, McSlotObjectWrapper::build(receiver, callback));
        return *this;
    }
    template<typename Func>
    McCppPromise &then(Func callback) noexcept
    {
        if constexpr (int(QtPrivate::FunctionPointer<Func>::ArgumentCount) >= 0) {
            static_assert(int(QtPrivate::FunctionPointer<Func>::ArgumentCount) <= 1,
                "The number of parameters of callback function can only be less than or equal to 1");
        } else {
            static_assert(int(McPrivate::LambdaType<Func>::ArgumentCount) <= 1,
                "The number of parameters of callback function can only be less than or equal to 1");
        }

        setCallback(CallbackType::Sync, McSlotObjectWrapper::build(nullptr, callback));
        return *this;
    }

    template<typename Func>
    McCppPromise &syncThen(const typename QtPrivate::FunctionPointer<Func>::Object *receiver, Func callback) noexcept
    {
        static_assert(int(QtPrivate::FunctionPointer<Func>::ArgumentCount) <= 1,
            "The number of parameters of callback function can only be less than or equal to 1");

        setCallback(CallbackType::Sync, McSlotObjectWrapper::build(receiver, callback));
        return *this;
    }
    template<typename Func>
    McCppPromise &syncThen(Func callback) noexcept
    {
        if constexpr (int(QtPrivate::FunctionPointer<Func>::ArgumentCount) >= 0) {
            static_assert(int(QtPrivate::FunctionPointer<Func>::ArgumentCount) <= 1,
                "The number of parameters of callback function can only be less than or equal to 1");
        } else {
            static_assert(int(McPrivate::LambdaType<Func>::ArgumentCount) <= 1,
                "The number of parameters of callback function can only be less than or equal to 1");
        }

        setCallback(CallbackType::Sync, McSlotObjectWrapper::build(nullptr, callback));
        return *this;
    }

    template<typename Func>
    McCppPromise &asyncThen(const typename QtPrivate::FunctionPointer<Func>::Object *receiver, Func callback) noexcept
    {
        static_assert(int(QtPrivate::FunctionPointer<Func>::ArgumentCount) <= 1,
            "The number of parameters of callback function can only be less than or equal to 1");

        setCallback(CallbackType::Async, McSlotObjectWrapper::build(receiver, callback));
        return *this;
    }
    template<typename Func>
    McCppPromise &asyncThen(Func callback) noexcept
    {
        if constexpr (int(QtPrivate::FunctionPointer<Func>::ArgumentCount) >= 0) {
            static_assert(int(QtPrivate::FunctionPointer<Func>::ArgumentCount) <= 1,
                "The number of parameters of callback function can only be less than or equal to 1");
        } else {
            static_assert(int(McPrivate::LambdaType<Func>::ArgumentCount) <= 1,
                "The number of parameters of callback function can only be less than or equal to 1");
        }

        setCallback(CallbackType::Async, McSlotObjectWrapper::build(nullptr, callback));
        return *this;
    }

    //! \note 只有调用此类的cancel方法或使用此类的getCancel返回的McCancel类才会使onCanceled回调生效
    template<typename Func>
    McCppPromise &onCanceled(const typename QtPrivate::FunctionPointer<Func>::Object *receiver, Func callback) noexcept
    {
        static_assert(int(QtPrivate::FunctionPointer<Func>::ArgumentCount) <= 1,
            "The number of parameters of callback function can only be less than or equal to 1");

        setCallback(CallbackType::Canceled, McSlotObjectWrapper::build(receiver, callback));
        return *this;
    }
    template<typename Func>
    McCppPromise &onCanceled(Func callback) noexcept
    {
        if constexpr (int(QtPrivate::FunctionPointer<Func>::ArgumentCount) >= 0) {
            static_assert(int(QtPrivate::FunctionPointer<Func>::ArgumentCount) <= 1,
                "The number of parameters of callback function can only be less than or equal to 1");
        } else {
            static_assert(int(McPrivate::LambdaType<Func>::ArgumentCount) <= 1,
                "The number of parameters of callback function can only be less than or equal to 1");
        }

        setCallback(CallbackType::Canceled, McSlotObjectWrapper::build(nullptr, callback));
        return *this;
    }

    template<typename Func>
    McCppPromise &onError(const typename QtPrivate::FunctionPointer<Func>::Object *receiver, Func callback) noexcept
    {
        static_assert(int(QtPrivate::FunctionPointer<Func>::ArgumentCount) <= 1,
            "The number of parameters of callback function can only be less than or equal to 1");

        setCallback(CallbackType::Error, McSlotObjectWrapper::build(receiver, callback));
        return *this;
    }
    template<typename Func>
    McCppPromise &onError(Func callback) noexcept
    {
        if constexpr (int(QtPrivate::FunctionPointer<Func>::ArgumentCount) >= 0) {
            static_assert(int(QtPrivate::FunctionPointer<Func>::ArgumentCount) <= 1,
                "The number of parameters of callback function can only be less than or equal to 1");
        } else {
            static_assert(int(McPrivate::LambdaType<Func>::ArgumentCount) <= 1,
                "The number of parameters of callback function can only be less than or equal to 1");
        }

        setCallback(CallbackType::Error, McSlotObjectWrapper::build(nullptr, callback));
        return *this;
    }

    template<typename Func>
    McCppPromise &onCurrentProgress(
        const typename QtPrivate::FunctionPointer<Func>::Object *recever, Func callback) noexcept
    {
        getProgress().onCurrentCallback(recever, callback);
        return *this;
    }
    template<typename Func>
    McCppPromise &onCurrentProgress(Func callback) noexcept
    {
        getProgress().onCurrentCallback(callback);
        return *this;
    }
    template<typename Func>
    McCppPromise &onTotalProgress(
        const typename QtPrivate::FunctionPointer<Func>::Object *recever, Func callback) noexcept
    {
        getProgress().onTotalCallback(recever, callback);
        return *this;
    }
    template<typename Func>
    McCppPromise &onTotalProgress(Func callback) noexcept
    {
        getProgress().onTotalCallback(callback);
        return *this;
    }

protected:
    void callCallback() noexcept override;
    void callCanceled() noexcept override;
    void callError() noexcept override;

private:
    void setCallback(CallbackType type, const McSlotObjectWrapper &functor) noexcept;

private:
    MC_DECL_PRIVATE(McCppPromise)
};
