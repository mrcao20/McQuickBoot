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
#define MC_QML_SOCKET_TAG "QmlSocket"
//!< Q_CLASSINFO

#define MC_CONTROLLER Q_CLASSINFO(MC_COMPONENT_TAG, MC_CONTROLLER_TAG)
#define MC_MODEL Q_CLASSINFO(MC_COMPONENT_TAG, MC_MODEL_TAG)
#define MC_QML_SOCKET Q_CLASSINFO(MC_COMPONENT_TAG, MC_QML_SOCKET_TAG)
