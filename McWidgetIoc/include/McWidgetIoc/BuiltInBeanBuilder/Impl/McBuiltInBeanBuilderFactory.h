#pragma once

#include "../IMcBuiltInBeanBuilderFactory.h"

#include "McBuiltInBeanBuilder.h"

template<typename T>
class McBuiltInBeanBuilderFactory : public IMcBuiltInBeanBuilderFactory
{
    Q_STATIC_ASSERT_X(!std::is_pointer<T>::value, "T must not be a pointer type");
    Q_STATIC_ASSERT_X(QtPrivate::IsPointerToTypeDerivedFromQObject<T *>::Value,
                      "T must be inherits from QObject");
    using Type = T;

public:
    using BuildFuncType = typename McBuiltInBeanBuilder<Type>::BuildFuncType;

    McBuiltInBeanBuilderFactory(const BuildFuncType &func) : m_func(func) {}

    IMcBuiltInBeanBuilderPtr create() noexcept override
    {
        return McBuiltInBeanBuilderPtr<Type>::create(m_func);
    }

private:
    BuildFuncType m_func;
};

template<typename T>
using McBuiltInBeanBuilderFactoryPtr = QSharedPointer<McBuiltInBeanBuilderFactory<T>>;
