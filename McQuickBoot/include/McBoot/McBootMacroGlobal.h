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

//! QML Socket
# define MC_ON_OPEN
# define MC_ON_CLOSE
# define MC_ON_ERROR
# define MC_ON_MESSAGE
//!< end QML Socket

//! 声明式bean
#define MC_BEAN
//!< end

#endif //! !Q_MOC_RUN

//! Q_CLASSINFO
#define MC_CONTROLLER_TAG "Controller"
#define MC_MODEL_TAG "Model"
#define MC_SERVICE_TAG "Service"
#define MC_CONFIGURATION_TAG "Configuration"
#define MC_CONFIGURATION_PRIORITY_TAG "ConfigurationPriority"
#define MC_CONFIGURATION_PROPERTIES_TAG "ConfigurationProperties"
#define MC_PROPERTY_SOURCE_TAG "PropertySource"
#define MC_QML_SOCKET_TAG "QmlSocket"
#define MC_SERIALIZATION_TAG "Serialization"
#define MC_JSON_SERIALIZATION_TAG "JsonSerialization"

#define MC_CONTROLLER Q_CLASSINFO(MC_COMPONENT_TAG, MC_CONTROLLER_TAG)
#define MC_MODEL Q_CLASSINFO(MC_COMPONENT_TAG, MC_MODEL_TAG)
#define MC_SERVICE Q_CLASSINFO(MC_COMPONENT_TAG, MC_SERVICE_TAG)
#define MC_CONFIGURATION(...) \
    Q_CLASSINFO(MC_COMPONENT_TAG, MC_CONFIGURATION_TAG) \
    Q_CLASSINFO(MC_CONFIGURATION_PRIORITY_TAG, MC_STRINGIFY("@" __VA_ARGS__))
#define MC_CONFIGURATION_PROPERTIES(arg) Q_CLASSINFO(MC_CONFIGURATION_PROPERTIES_TAG, arg)
#define MC_PROPERTY_SOURCE(arg) Q_CLASSINFO(MC_PROPERTY_SOURCE_TAG, arg)
#define MC_QML_SOCKET Q_CLASSINFO(MC_COMPONENT_TAG, MC_QML_SOCKET_TAG)
#define MC_JSON_SERIALIZATION Q_CLASSINFO(MC_SERIALIZATION_TAG, MC_JSON_SERIALIZATION_TAG)
//!< Q_CLASSINFO

#define MC_QUICKBOOT_WORK_THREAD_NAME __mc__quickBootWorkThread
#define MC_DECL_QUICKBOOT_WORK_THREAD \
    Q_INVOKABLE \
    MC_BEAN \
    QThread *MC_QUICKBOOT_WORK_THREAD_NAME() const noexcept;

#define MC_SIMPLE_DEFINE_QUICKBOOT_WORK_THREAD(Class) \
    QThread *Class::MC_QUICKBOOT_WORK_THREAD_NAME() const noexcept \
    { \
        return new QThread(); \
    }

#define MC_CUSTOM_DEFINE_QUICKBOOT_WORK_THREAD(Class, par) \
    QThread *Class::MC_QUICKBOOT_WORK_THREAD_NAME() const noexcept par

#define MC_CUSTOM_FUNC_DEFINE_QUICKBOOT_WORK_THREAD(Class, func) \
    QThread *Class::MC_QUICKBOOT_WORK_THREAD_NAME() const noexcept { return func(); }
