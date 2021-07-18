#pragma once

#include <McIoc/McMacroGlobal.h>

#if defined (MC_BUILD_STATIC) && !defined (MC_EXPORT_DISABLE)
#define MC_EXPORT_DISABLE
#endif

#ifndef MC_EXPORT_DISABLE
# if defined(MCQUICKBOOT_LIBRARY)
#  define MCQUICKBOOT_EXPORT Q_DECL_EXPORT
# else
#  define MCQUICKBOOT_EXPORT Q_DECL_IMPORT
# endif
#else
# define MCQUICKBOOT_EXPORT
#endif

#ifndef Q_MOC_RUN            //!< 这行语句必须加，只有包围在这行语句之中的宏才能被识别为tag

//! 声明式bean
#define MC_BEAN
//!< end

#endif //! !Q_MOC_RUN

//! Q_CLASSINFO
#define MC_CONTROLLER_TAG "McController"
#define MC_MODEL_TAG "McModel"
#define MC_SERVICE_TAG "McService"
#define MC_CONFIGURATION_TAG "McConfiguration"
#define MC_CONFIGURATION_PRIORITY_TAG "McConfigurationPriority"
#define MC_CONFIGURATION_PROPERTIES_TAG "McConfigurationProperties"
#define MC_PROPERTY_SOURCE_TAG "McPropertySource"
#define MC_SERIALIZATION_TAG "McSerialization"
#define MC_JSON_SERIALIZATION_TAG "McJsonSerialization"

#define MC_CONTROLLER(...) \
    MC_BEANNAME("" __VA_ARGS__) \
    Q_CLASSINFO(MC_COMPONENT_TAG, MC_CONTROLLER_TAG)
#define MC_MODEL(...) \
    MC_BEANNAME("" __VA_ARGS__) \
    Q_CLASSINFO(MC_COMPONENT_TAG, MC_MODEL_TAG)
#define MC_SERVICE(...) \
    MC_BEANNAME("" __VA_ARGS__) \
    Q_CLASSINFO(MC_COMPONENT_TAG, MC_SERVICE_TAG)
#define MC_CONFIGURATION(...) \
    Q_CLASSINFO(MC_COMPONENT_TAG, MC_CONFIGURATION_TAG) \
    Q_CLASSINFO(MC_CONFIGURATION_PRIORITY_TAG, MC_STRINGIFY("@" __VA_ARGS__))
#define MC_CONFIGURATION_PROPERTIES(arg) Q_CLASSINFO(MC_CONFIGURATION_PROPERTIES_TAG, arg)
#define MC_PROPERTY_SOURCE(arg) Q_CLASSINFO(MC_PROPERTY_SOURCE_TAG, arg)
#define MC_JSON_SERIALIZATION() Q_CLASSINFO(MC_SERIALIZATION_TAG, MC_JSON_SERIALIZATION_TAG)
//!< Q_CLASSINFO

// Work Thread
#define MC_QUICKBOOT_WORK_THREAD_NAME __mc__quickBootWorkThread
#define MC_INLINE_QUICKBOOT_WORK_THREAD() \
    Q_INVOKABLE \
    MC_BEAN \
    QThread *MC_QUICKBOOT_WORK_THREAD_NAME() const noexcept { return new QThread(); }

#define MC_DECL_QUICKBOOT_WORK_THREAD() \
    Q_INVOKABLE \
    MC_BEAN \
    QThread *MC_QUICKBOOT_WORK_THREAD_NAME() const noexcept;

#define MC_SIMPLE_DEFINE_QUICKBOOT_WORK_THREAD(Class) \
    QThread *Class::MC_QUICKBOOT_WORK_THREAD_NAME() const noexcept { return new QThread(); }

#define MC_CUSTOM_DEFINE_QUICKBOOT_WORK_THREAD(Class, par) \
    QThread *Class::MC_QUICKBOOT_WORK_THREAD_NAME() const noexcept par

#define MC_CUSTOM_FUNC_DEFINE_QUICKBOOT_WORK_THREAD(Class, func) \
    QThread *Class::MC_QUICKBOOT_WORK_THREAD_NAME() const noexcept { return func(); }

// State Machine
#define MC_QUICKBOOT_STATE_MACHINE_NAME __mc__quickBootStateMachine
#define MC_INLINE_QUICKBOOT_STATE_MACHINE(Class) \
    Q_INVOKABLE \
    MC_BEAN \
    QScxmlStateMachine *MC_QUICKBOOT_STATE_MACHINE_NAME() const noexcept { return new Class(); }

#define MC_DECL_QUICKBOOT_STATE_MACHINE() \
    Q_INVOKABLE \
    MC_BEAN \
    QScxmlStateMachine *MC_QUICKBOOT_STATE_MACHINE_NAME() const noexcept;

#define MC_SIMPLE_DEFINE_QUICKBOOT_STATE_MACHINE(Class, Class2) \
    QScxmlStateMachine *Class::MC_QUICKBOOT_STATE_MACHINE_NAME() const noexcept \
    { \
        return new Class2(); \
    }

#define MC_CUSTOM_DEFINE_QUICKBOOT_STATE_MACHINE(Class, par) \
    QScxmlStateMachine *Class::MC_QUICKBOOT_STATE_MACHINE_NAME() const noexcept par

#define MC_CUSTOM_FUNC_DEFINE_QUICKBOOT_STATE_MACHINE(Class, func) \
    QScxmlStateMachine *Class::MC_QUICKBOOT_STATE_MACHINE_NAME() const noexcept { return func(); }
