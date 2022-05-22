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

#include "McAbstractBeanBuilder.h"

template<typename T>
class McCustomBeanBuilder : public McAbstractBeanBuilder
{
    static_assert(!std::is_pointer<T>::value, "T must not be a pointer type");
    using Type = T;

public:
    using BuildFuncType = std::function<void(Type *, const QString &, const QVariant &)>;

    McCustomBeanBuilder(const BuildFuncType &func, bool isShared)
        : m_func(func)
        , m_isShared(isShared)
    {
    }

    bool isPointer() const noexcept override { return !m_isShared; }

protected:
    QVariant create() noexcept override
    {
        if (m_isShared) {
            return QVariant::fromValue(QSharedPointer<Type>::create());
        } else {
            return QVariant::fromValue(new Type());
        }
    }

    void complete(QVariant &bean, QThread *thread) noexcept override
    {
        Type *obj = nullptr;
        if (m_isShared) {
            obj = bean.value<QSharedPointer<Type>>().data();
        } else {
            obj = bean.value<Type *>();
        }
        if (Q_UNLIKELY(obj == nullptr)) {
            return;
        }
        auto properties = buildProperties(bean);
        QMapIterator<QString, QVariant> itr(properties);
        while (itr.hasNext()) {
            auto item = itr.next();
            auto key = item.key();
            auto value = item.value();

            if (m_func == nullptr) {
                if constexpr (QtPrivate::IsPointerToTypeDerivedFromQObject<Type *>::Value) {
                    obj->setProperty(key.toLatin1(), value);
                }
            } else {
                m_func(obj, key, value);
            }
        }
        if constexpr (QtPrivate::IsPointerToTypeDerivedFromQObject<Type *>::Value) {
            if (thread != nullptr && thread != obj->thread()) {
                obj->moveToThread(thread);
            }
        }
    }

    void doMoveToThread(const QVariant &bean, QThread *thread, const QVariantHash &properties) noexcept override
    {
        Q_UNUSED(bean)
        Q_UNUSED(thread)
        Q_UNUSED(properties)
    }

private:
    bool m_isShared{false};
    BuildFuncType m_func;
};

template<typename T>
using McCustomBeanBuilderPtr = QSharedPointer<McCustomBeanBuilder<T>>;
