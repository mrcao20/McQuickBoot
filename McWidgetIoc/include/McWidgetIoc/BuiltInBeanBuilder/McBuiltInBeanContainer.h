#pragma once

#include "../McWidgetGlobal.h"

#include "Impl/McBuiltInBeanBuilderFactory.h"

class MCWIDGETIOC_EXPORT McBuiltInBeanContainer
{
public:
    static void setBuilderFactory(const QString &className,
                                  IMcBuiltInBeanBuilderFactoryConstPtrRef factory) noexcept;
    static IMcBuiltInBeanBuilderPtr getBuilder(const QString &className) noexcept;

    template<typename T>
    static void setBuilderFactory(const typename McBuiltInBeanBuilder<T>::BuildFuncType &func) noexcept
    {
        qRegisterMetaType<T *>();
        using FactoryType = McBuiltInBeanBuilderFactoryPtr<T>;
        QByteArray className = T::staticMetaObject.className();
        setBuilderFactory(className, FactoryType::create(func));
    }
};
