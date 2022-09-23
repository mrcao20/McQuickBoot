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

#define MC_TYPELIST(...) \
 public: \
 using McPrivateTypeList = McPrivate::McTypeList<__VA_ARGS__>; \
 using McPrivateTypeListHelper = void; \
\
 private:

#define MC_INTERFACES(...) MC_TYPELIST(__VA_ARGS__)

namespace McPrivate {
template<typename...>
struct McTypeList;

template<typename T, typename... U>
struct McTypeList<T, U...>
{
    using Head = T;
    using Tails = McTypeList<U...>;
};

// 针对空list的特化
template<>
struct McTypeList<>
{
};
} // namespace McPrivate

template<typename T>
inline void mcRegisterMetaTypeSimple() noexcept
{
}
