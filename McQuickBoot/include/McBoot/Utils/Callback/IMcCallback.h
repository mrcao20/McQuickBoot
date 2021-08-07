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

#include <McIoc/McGlobal.h>

class IMcCallback
{
public:
    struct McCallbackDeleter
    {
        void operator()(IMcCallback *ptr) { ptr->destroy(); }
    };

    virtual ~IMcCallback() = default;

    virtual void destroy() noexcept = 0;
    virtual void call(const QVariantList &varList) noexcept = 0;

    void call(const QVariant &var) noexcept { call(QVariantList() << var); }

    template<typename... Args>
    void call(const Args &... args) noexcept
    {
        QVariantList vars;
        (vars << ... << toQVariant(args));
        call(vars);
    }

    void operator()(const QVariantList &varList) noexcept { call(varList); }

    void operator()(const QVariant &var) noexcept { call(var); }

    template<typename... Args>
    void operator()(const Args &... args) noexcept
    {
        call(args...);
    }

private:
    template<typename T>
    typename std::enable_if<McPrivate::IsQVariantHelper<T>::Value, QVariant>::type toQVariant(
        const T &t)
    {
        return QVariant::fromValue(t);
    }
    QVariant toQVariant(const QVariant &t) { return t; }
    template<typename T>
    typename std::enable_if<McPrivate::IsQVariantHelper<T>::Value2, QVariant>::type toQVariant(
        const T &t)
    {
        return QVariant(t);
    }
};

MC_DECL_METATYPE(IMcCallback)
