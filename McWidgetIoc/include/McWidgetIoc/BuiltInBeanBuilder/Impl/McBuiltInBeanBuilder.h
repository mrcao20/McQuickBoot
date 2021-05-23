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
