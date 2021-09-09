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

#include <QObject>
#include <QSharedData>

#include "Callback/Impl/McCppSyncCallback.h"

struct MCQUICKBOOT_EXPORT McProgressSharedData : public QSharedData
{
    QAtomicInt current{0};
    QAtomicInt total{100};
    IMcCallbackPtr callback;
};

class MCQUICKBOOT_EXPORT McProgress
{
    MC_DECL_INIT(McProgress)
public:
    McProgress() noexcept;
    ~McProgress();

    int current() const noexcept;
    void setCurrent(int val) noexcept;
    int total() const noexcept;
    void setTotal(int val) noexcept;

    template<typename Func>
    void callback(const typename QtPrivate::FunctionPointer<Func>::Object *recever,
                  Func func) noexcept
    {
        typedef QtPrivate::FunctionPointer<Func> FuncType;

        Q_STATIC_ASSERT_X(int(FuncType::ArgumentCount) == 2,
                          "The number of parameters of callback function can only be equal to "
                          "2(int current, int total)");

        setCallback(McCppSyncCallback::build(recever, func));
    }
    template<typename Func>
    typename std::enable_if<int(QtPrivate::FunctionPointer<Func>::ArgumentCount) >= 0
                                && !QtPrivate::FunctionPointer<Func>::IsPointerToMemberFunction,
                            void>::type
    callback(Func func) noexcept
    {
        typedef QtPrivate::FunctionPointer<Func> FuncType;

        Q_STATIC_ASSERT_X(int(FuncType::ArgumentCount) == 2,
                          "The number of parameters of callback function can only be equal to "
                          "2(int current, int total)");

        setCallback(McCppSyncCallback::build(func));
    }
    template<typename Func>
    typename std::enable_if<QtPrivate::FunctionPointer<Func>::ArgumentCount == -1, void>::type
    callback(Func func) noexcept
    {
        typedef McPrivate::LambdaType<Func> FuncType;

        Q_STATIC_ASSERT_X(int(FuncType::ArgumentCount) == 2,
                          "The number of parameters of callback function can only be equal to "
                          "2(int current, int total)");

        setCallback(McCppSyncCallback::build(func));
    }

private:
    void setCallback(const IMcCallbackPtr &val) noexcept;
    void callCallback() noexcept;

private:
    QExplicitlySharedDataPointer<McProgressSharedData> d;
};

Q_DECLARE_METATYPE(McProgress)
