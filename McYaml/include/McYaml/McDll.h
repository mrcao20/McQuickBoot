#pragma once

#include <QtCore/qglobal.h>

#if defined(MCYAML_LIBRARY)
#  define MCYAML_EXPORT Q_DECL_EXPORT
#else
#  define MCYAML_EXPORT Q_DECL_IMPORT
#endif
