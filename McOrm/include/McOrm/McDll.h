#pragma once

#include <QtCore/qglobal.h>

#if defined(MCORM_LIBRARY)
#  define MCORM_EXPORT Q_DECL_EXPORT
#else
#  define MCORM_EXPORT Q_DECL_IMPORT
#endif
