/*
 * MIT License
 *
 * Copyright (c) 2021 mrcao20
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#pragma once

#include <QSharedData>

#include "../Callback/Impl/McCppAsyncCallback.h"
#include "../McGlobal.h"

struct McPauseSharedData;

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
    QExplicitlySharedDataPointer<McPauseSharedData> d;
};

MC_DECL_METATYPE(McPause)
