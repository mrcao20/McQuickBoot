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

#include "../Callback/Impl/McCppSyncCallback.h"

MC_FORWARD_DECL_PRIVATE_DATA(McProgress)

class MC_CORE_EXPORT McProgress
{
    struct CurrentChecker
    {
        template<int argCount>
        static inline void check() noexcept
        {
            static_assert(
                argCount == 1, "The number of parameters of callback function can only be equal to 1(int current)");
        }
    };

    struct TotalChecker
    {
        template<int argCount>
        static inline void check() noexcept
        {
            static_assert(
                argCount == 1, "The number of parameters of callback function can only be equal to 1(int total)");
        }
    };

public:
    McProgress() noexcept;
    ~McProgress();

    McProgress(const McProgress &o) noexcept;
    McProgress &operator=(const McProgress &o) noexcept;
    McProgress(McProgress &&o) noexcept;
    McProgress &operator=(McProgress &&o) noexcept;

    int current() const noexcept;
    void setCurrent(int val) noexcept;
    void increaseCurrent(int val = 1) noexcept;
    int total() const noexcept;
    void setTotal(int val) noexcept;
    void setCurrentCallback(const IMcCallbackPtr &val) noexcept;
    void setTotalCallback(const IMcCallbackPtr &val) noexcept;

    template<typename Object, typename Func>
    void onCurrentCallback(const Object *receiver, Func callback) noexcept
    {
        setCurrentCallback(McCppSyncCallback::build<Object, Func, CurrentChecker>(receiver, callback));
    }
    template<typename Func>
    void onCurrentCallback(Func callback) noexcept
    {
        setCurrentCallback(McCppSyncCallback::build<Func, CurrentChecker>(callback));
    }

    template<typename Object, typename Func>
    void onTotalCallback(const Object *receiver, Func callback) noexcept
    {
        setTotalCallback(McCppSyncCallback::build<Object, Func, TotalChecker>(receiver, callback));
    }
    template<typename Func>
    void onTotalCallback(Func callback) noexcept
    {
        setTotalCallback(McCppSyncCallback::build<Func, TotalChecker>(callback));
    }

private:
    MC_DECL_SHARED_PRIVATE(McProgress)
};

#ifdef MC_USE_QT5
Q_DECLARE_METATYPE(McProgress)
#endif
