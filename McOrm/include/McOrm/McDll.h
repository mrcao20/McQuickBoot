#pragma once

#include <QtCore/qglobal.h>

#if defined (MC_BUILD_STATIC) && !defined (MC_EXPORT_DISABLE)
#define MC_EXPORT_DISABLE
#endif

#ifndef MC_EXPORT_DISABLE
# if defined(MCORM_LIBRARY)
#  define MCORM_EXPORT Q_DECL_EXPORT
# else
#  define MCORM_EXPORT Q_DECL_IMPORT
# endif
#else
# define MCORM_EXPORT
#endif
