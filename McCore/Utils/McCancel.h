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

#include <QSharedData>

#include "../Callback/Impl/McCppAsyncCallback.h"
#include "../McGlobal.h"

MC_FORWARD_DECL_PRIVATE_DATA(McCancel)

class MC_CORE_EXPORT McCancel
{
    MC_DECL_INIT(McCancel)
public:
    McCancel() noexcept;
    ~McCancel();

    McCancel(const McCancel &o) noexcept;
    McCancel &operator=(const McCancel &o) noexcept;
    McCancel(McCancel &&o) noexcept;
    McCancel &operator=(McCancel &&o) noexcept;

    bool isCanceled() const noexcept;
    void cancel() noexcept;
    QAtomicInteger<bool> &capture() noexcept;
    const QAtomicInteger<bool> &capture() const noexcept;
    void setCallback(const IMcCallbackPtr &val) noexcept;

    template<typename Func>
    void callback(const typename QtPrivate::FunctionPointer<Func>::Object *recever, Func func) noexcept
    {
        static_assert(int(QtPrivate::FunctionPointer<Func>::ArgumentCount) <= 1,
            "The number of parameters of callback function can only be less than or equal to 1(bool isCanceled)");

        setCallback(McCppAsyncCallback::build(recever, func));
    }
    template<typename Func>
    std::enable_if_t<int(QtPrivate::FunctionPointer<Func>::ArgumentCount) >= 0
                         && !QtPrivate::FunctionPointer<Func>::IsPointerToMemberFunction,
        void>
        callback(Func func) noexcept
    {
        static_assert(int(QtPrivate::FunctionPointer<Func>::ArgumentCount) <= 1,
            "The number of parameters of callback function can only be less than or equal to 1(bool isCanceled)");

        setCallback(McCppAsyncCallback::build(func));
    }
    template<typename Func>
    std::enable_if_t<QtPrivate::FunctionPointer<Func>::ArgumentCount == -1, void> callback(Func func) noexcept
    {
        static_assert(int(McPrivate::LambdaType<Func>::ArgumentCount) <= 1,
            "The number of parameters of callback function can only be less than or equal to 1(bool isCanceled)");

        setCallback(McCppAsyncCallback::build(func));
    }

private:
    MC_DECL_SHARED_PRIVATE(McCancel)
};

MC_DECL_METATYPE(McCancel)
