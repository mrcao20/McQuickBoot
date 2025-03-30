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

MC_FORWARD_DECL_PRIVATE_DATA(McPause)

class MC_CORE_EXPORT McPause
{
    struct PauseChecker
    {
        template<int argCount>
        static inline void check() noexcept
        {
            static_assert(argCount <= 1,
                "The number of parameters of callback function can only be less than or equal to 1(bool isPaused)");
        }
    };

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

    template<typename Object, typename Func>
    void onCallback(const Object *receiver, Func callback) noexcept
    {
        setCallback(McCppAsyncCallback::build<Object, Func, PauseChecker>(receiver, callback));
    }
    template<typename Func>
    void onCallback(Func callback) noexcept
    {
        setCallback(McCppAsyncCallback::build<Func, PauseChecker>(callback));
    }

private:
    MC_DECL_SHARED_PRIVATE(McPause)
};

#ifdef MC_USE_QT5
Q_DECLARE_METATYPE(McPause)
#endif
