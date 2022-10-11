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
#include "../McGlobal.h"

struct McProgressSharedData;

class MC_CORE_EXPORT McProgress
{
    MC_DECL_INIT(McProgress)
public:
    McProgress() noexcept;
    ~McProgress();

    McProgress(const McProgress &o) noexcept;
    McProgress &operator=(const McProgress &o) noexcept;
    McProgress(McProgress &&o) noexcept;
    McProgress &operator=(McProgress &&o) noexcept;

    int current() const noexcept;
    void setCurrent(int val) noexcept;
    int total() const noexcept;
    void setTotal(int val) noexcept;
    void setCurrentCallback(const IMcCallbackPtr &val) noexcept;
    void setTotalCallback(const IMcCallbackPtr &val) noexcept;

    template<typename Func>
    void currentCallback(const typename QtPrivate::FunctionPointer<Func>::Object *recever, Func func) noexcept
    {
        static_assert(int(QtPrivate::FunctionPointer<Func>::ArgumentCount) == 1,
            "The number of parameters of callback function can only be equal to 1(int current)");

        setCurrentCallback(McCppSyncCallback::build(recever, func));
    }
    template<typename Func>
    std::enable_if_t<int(QtPrivate::FunctionPointer<Func>::ArgumentCount) >= 0
                         && !QtPrivate::FunctionPointer<Func>::IsPointerToMemberFunction,
        void>
        currentCallback(Func func) noexcept
    {
        static_assert(int(QtPrivate::FunctionPointer<Func>::ArgumentCount) == 1,
            "The number of parameters of callback function can only be equal to 1(int current)");

        setCurrentCallback(McCppSyncCallback::build(func));
    }
    template<typename Func>
    std::enable_if_t<QtPrivate::FunctionPointer<Func>::ArgumentCount == -1, void> currentCallback(Func func) noexcept
    {
        static_assert(int(McPrivate::LambdaType<Func>::ArgumentCount) == 1,
            "The number of parameters of callback function can only be equal to 1(int current)");

        setCurrentCallback(McCppSyncCallback::build(func));
    }

    template<typename Func>
    void totalCallback(const typename QtPrivate::FunctionPointer<Func>::Object *recever, Func func) noexcept
    {
        static_assert(int(QtPrivate::FunctionPointer<Func>::ArgumentCount) == 1,
            "The number of parameters of callback function can only be equal to 1(int total)");

        setTotalCallback(McCppSyncCallback::build(recever, func));
    }
    template<typename Func>
    std::enable_if_t<int(QtPrivate::FunctionPointer<Func>::ArgumentCount) >= 0
                         && !QtPrivate::FunctionPointer<Func>::IsPointerToMemberFunction,
        void>
        totalCallback(Func func) noexcept
    {
        static_assert(int(QtPrivate::FunctionPointer<Func>::ArgumentCount) == 1,
            "The number of parameters of callback function can only be equal to 1(int total)");

        setTotalCallback(McCppSyncCallback::build(func));
    }
    template<typename Func>
    std::enable_if_t<QtPrivate::FunctionPointer<Func>::ArgumentCount == -1, void> totalCallback(Func func) noexcept
    {
        static_assert(int(McPrivate::LambdaType<Func>::ArgumentCount) == 1,
            "The number of parameters of callback function can only be equal to 1(int total)");

        setTotalCallback(McCppSyncCallback::build(func));
    }

private:
    QExplicitlySharedDataPointer<McProgressSharedData> d;
};

MC_DECL_METATYPE(McProgress)
