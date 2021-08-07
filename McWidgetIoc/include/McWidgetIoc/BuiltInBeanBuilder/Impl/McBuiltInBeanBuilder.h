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

#include "../IMcBuiltInBeanBuilder.h"

template<typename T>
class McBuiltInBeanBuilder : public IMcBuiltInBeanBuilder
{
    Q_STATIC_ASSERT_X(!std::is_pointer<T>::value, "T must not be a pointer type");
    using Type = T;

public:
    using BuildFuncType = std::function<void(Type *, const QString &, const QVariant &)>;

    McBuiltInBeanBuilder(const BuildFuncType &func) : m_func(func) {}

    QWidget *build() noexcept override
    {
        if (m_bean != nullptr) {
            return m_bean;
        }
        m_bean = new Type();
        return m_bean;
    }
    void setProperty(const QString &name, const QVariant &val) noexcept override
    {
        if (m_bean == nullptr) {
            return;
        }
        m_func(m_bean, name, val);
    }

private:
    Type *m_bean{nullptr};
    BuildFuncType m_func;
};

template<typename T>
using McBuiltInBeanBuilderPtr = QSharedPointer<McBuiltInBeanBuilder<T>>;
