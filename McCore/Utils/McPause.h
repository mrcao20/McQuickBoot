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

MC_FORWARD_DECL_PRIVATE_DATA(McPause)

class MC_CORE_EXPORT McPause
{
    MC_DECL_INIT(McPause)
public:
    McPause() noexcept;
    ~McPause();

    McPause(const McPause &o) noexcept;
    McPause &operator=(const McPause &o) noexcept;
    McPause(McPause &&o) noexcept;
    McPause &operator=(McPause &&o) noexcept;

    bool isPaused() const noexcept;
    void pause() noexcept;
    void resume() noexcept;
    void setPaused(bool val) noexcept;
    QAtomicInteger<bool> &capture() noexcept;
    const QAtomicInteger<bool> &capture() const noexcept;
    void setCallback(const IMcCallbackPtr &val) noexcept;

    template<typename Func>
    void callback(const typename QtPrivate::FunctionPointer<Func>::Object *recever, Func func) noexcept
    {
        static_assert(int(QtPrivate::FunctionPointer<Func>::ArgumentCount) <= 1,
            "The number of parameters of callback function can only be less than or equal to 1(bool isPaused)");

        setCallback(McCppAsyncCallback::build(recever, func));
    }
    template<typename Func>
    std::enable_if_t<int(QtPrivate::FunctionPointer<Func>::ArgumentCount) >= 0
                         && !QtPrivate::FunctionPointer<Func>::IsPointerToMemberFunction,
        void>
        callback(Func func) noexcept
    {
        static_assert(int(QtPrivate::FunctionPointer<Func>::ArgumentCount) <= 1,
            "The number of parameters of callback function can only be less than or equal to 1(bool isPaused)");

        setCallback(McCppAsyncCallback::build(func));
    }
    template<typename Func>
    std::enable_if_t<QtPrivate::FunctionPointer<Func>::ArgumentCount == -1, void> callback(Func func) noexcept
    {
        static_assert(int(McPrivate::LambdaType<Func>::ArgumentCount) <= 1,
            "The number of parameters of callback function can only be less than or equal to 1(bool isPaused)");

        setCallback(McCppAsyncCallback::build(func));
    }

private:
    MC_DECL_SHARED_PRIVATE(McPause)
};

MC_DECL_METATYPE(McPause)
