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

    struct PromiseChecker
    {
        template<int argCount>
        static inline void check() noexcept
        {
            static_assert(
                argCount <= 1, "The number of parameters of callback function can only be less than or equal to 1");
        }
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

    template<typename... Args>
    McCppPromise &then(Args &&...args) noexcept
    {
        setCallback(CallbackType::Sync, buildWrapper(std::forward<Args>(args)...));
        return *this;
    }

    template<typename... Args>
    McCppPromise &syncThen(Args &&...args) noexcept
    {
        setCallback(CallbackType::Sync, buildWrapper(std::forward<Args>(args)...));
        return *this;
    }

    template<typename... Args>
    McCppPromise &asyncThen(Args &&...args) noexcept
    {
        setCallback(CallbackType::Async, buildWrapper(std::forward<Args>(args)...));
        return *this;
    }

    //! \note 只有调用此类的cancel方法或使用此类的getCancel返回的McCancel类才会使onCanceled回调生效
    template<typename... Args>
    McCppPromise &onCanceled(Args &&...args) noexcept
    {
        setCallback(CallbackType::Canceled, buildWrapper(std::forward<Args>(args)...));
        return *this;
    }

    template<typename... Args>
    McCppPromise &onError(Args &&...args) noexcept
    {
        setCallback(CallbackType::Error, buildWrapper(std::forward<Args>(args)...));
        return *this;
    }

    template<typename... Args>
    McCppPromise &onCurrentProgress(Args &&...args) noexcept
    {
        getProgress().onCurrentCallback(std::forward<Args>(args)...);
        return *this;
    }
    template<typename... Args>
    McCppPromise &onTotalProgress(Args &&...args) noexcept
    {
        getProgress().onTotalCallback(std::forward<Args>(args)...);
        return *this;
    }

protected:
    void callCallback() noexcept override;
    void callCanceled() noexcept override;
    void callError() noexcept override;

private:
    void setCallback(CallbackType type, const McSlotObjectWrapper &functor) noexcept;

    template<typename Object, typename Func>
    McSlotObjectWrapper buildWrapper(const Object *receiver, Func callback) const noexcept
    {
        return McSlotObjectWrapper::build<Func, PromiseChecker>(receiver, callback);
    }
    template<typename Func>
    McSlotObjectWrapper buildWrapper(Func callback) const noexcept
    {
        return McSlotObjectWrapper::build<Func, PromiseChecker>(nullptr, std::move(callback));
    }

private:
    MC_DECL_PRIVATE(McCppPromise)
};
