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

#include <optional>

#include "../../Utils/McSlotObjectWrapper.h"
#include "McAbstractAsyncCallback.h"

class MC_CORE_EXPORT McCppAsyncCallback : public McAbstractAsyncCallback
{
    MC_DECL_INIT(McCppAsyncCallback)
    MC_INTERFACES(McAbstractAsyncCallback)
    MC_DECL_POINTER(McCppAsyncCallback)
public:
    using IMcCallback::call;

    McCppAsyncCallback(const McCppAsyncCallback &o) noexcept;
    McCppAsyncCallback &operator=(const McCppAsyncCallback &o) noexcept;
    McCppAsyncCallback(McCppAsyncCallback &&o) noexcept;
    McCppAsyncCallback &operator=(McCppAsyncCallback &&o) noexcept;

    void call(const QVariantList &varList) const noexcept override;

    template<typename Func>
    McCppAsyncCallback(const typename QtPrivate::FunctionPointer<Func>::Object *receiver, Func callback) noexcept
    {
        m_callback = std::make_optional(McSlotObjectWrapper::build(receiver, callback));
    }

    template<typename Func>
    McCppAsyncCallback(Func callback) noexcept
    {
        m_callback = std::make_optional(McSlotObjectWrapper::build(nullptr, std::move(callback)));
    }

    template<typename Func>
    static McCppAsyncCallbackPtr build(
        const typename QtPrivate::FunctionPointer<Func>::Object *receiver, Func callback) noexcept
    {
        return McCppAsyncCallbackPtr(new McCppAsyncCallback(receiver, callback), CallbackDeleter());
    }
    template<typename Func>
    static McCppAsyncCallbackPtr build(Func callback) noexcept
    {
        return McCppAsyncCallbackPtr(new McCppAsyncCallback(std::move(callback)), CallbackDeleter());
    }

private:
    std::optional<McSlotObjectWrapper> m_callback;
};

MC_DECL_METATYPE(McCppAsyncCallback)
