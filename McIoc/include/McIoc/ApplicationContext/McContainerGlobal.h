#pragma once

#include "../McGlobal.h"
#include "../ApplicationContext/impl/McAnnotationApplicationContext.h"


#define MC_REGISTER_COMPONENT(...) MC_REGISTER_COMPONENT_IMPL(__VA_ARGS__)
#define MC_REGISTER_COMPONENT_IMPL(...) \
    mcRegisterComponent<__VA_ARGS__>(MC_FIRST_TYPE_NAME(__VA_ARGS__), MC_FIRST_TYPE_NAME_CONST_REF(__VA_ARGS__));


//! 需要自动注入的类，则需要调用此函数，否则直接调用mcRegisterBeanFactory
template<typename T>
void mcRegisterComponent(const char *typeName, const char *constRefTypeName) 
{
    Q_ASSERT(typeName != nullptr && constRefTypeName != nullptr);
    mcRegisterBeanFactory<T>(typeName, constRefTypeName);
    McAnnotationApplicationContext::insertRegistry(typeName);
}

//! 需要自动注入的类，则需要调用此函数，否则直接调用mcRegisterBeanFactory
template<typename From, typename To>
void mcRegisterComponent(const char *typeName, const char *constRefTypeName) 
{
    Q_ASSERT(typeName != nullptr && constRefTypeName != nullptr);
    mcRegisterBeanFactory<From, To>(typeName, constRefTypeName);
    McAnnotationApplicationContext::insertRegistry(typeName);
}

MCIOC_EXPORT void mcConnect(
        const QString &beanName,
        const QString &sender,
        const QString &signal,
        const QString &receiver,
        const QString &slot,
        Qt::ConnectionType type = Qt::AutoConnection) noexcept;
