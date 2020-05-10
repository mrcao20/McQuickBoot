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
#define MC_COMPONENT "Component"

#define MC_CONTROLLER "Controller"
#define MC_MODEL "Model"
#define MC_QML_SOCKET "QmlSocket"
//!< Q_CLASSINFO
