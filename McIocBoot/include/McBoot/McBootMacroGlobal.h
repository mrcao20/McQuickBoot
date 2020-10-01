#pragma once

#include <McIoc/McMacroGlobal.h>

#if defined(MCIOCBOOT_LIBRARY)
#  define MCIOCBOOT_EXPORT Q_DECL_EXPORT
#else
#  define MCIOCBOOT_EXPORT Q_DECL_IMPORT
#endif


#ifndef Q_MOC_RUN			//!< 这行语句必须加，只有包围在这行语句之中的宏才能被识别为tag

//! QML Socket
# define MC_ON_OPEN
# define MC_ON_CLOSE
# define MC_ON_ERROR
# define MC_ON_MESSAGE
//!< end QML Socket

#endif //! !Q_MOC_RUN


//! Q_CLASSINFO
#define MC_CONTROLLER_TAG "Controller"
#define MC_MODEL_TAG "Model"
#define MC_SERVICE_TAG "Service"
#define MC_CONFIGURATION_TAG "Configuration"
#define MC_CONFIGURATION_PROPERTIES_TAG "ConfigurationProperties"
#define MC_PROPERTY_SOURCE_TAG "PropertySource"
#define MC_QML_SOCKET_TAG "QmlSocket"

#define MC_CONTROLLER Q_CLASSINFO(MC_COMPONENT_TAG, MC_CONTROLLER_TAG)
#define MC_MODEL Q_CLASSINFO(MC_COMPONENT_TAG, MC_MODEL_TAG)
#define MC_SERVICE Q_CLASSINFO(MC_COMPONENT_TAG, MC_SERVICE_TAG)
#define MC_CONFIGURATION Q_CLASSINFO(MC_COMPONENT_TAG, MC_CONFIGURATION_TAG)
#define MC_CONFIGURATION_PROPERTIES(arg) Q_CLASSINFO(MC_CONFIGURATION_PROPERTIES_TAG, arg)
#define MC_PROPERTY_SOURCE(arg) Q_CLASSINFO(MC_PROPERTY_SOURCE_TAG, arg)
#define MC_QML_SOCKET Q_CLASSINFO(MC_COMPONENT_TAG, MC_QML_SOCKET_TAG)
//!< Q_CLASSINFO
