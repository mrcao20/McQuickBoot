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
