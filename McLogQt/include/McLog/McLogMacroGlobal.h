#pragma once

#include <QtCore/qglobal.h>

#include <McIoc/McMacroGlobal.h>

#if defined (MC_BUILD_STATIC) && !defined (MC_EXPORT_DISABLE)
#define MC_EXPORT_DISABLE
#endif

#ifndef MC_EXPORT_DISABLE
# if defined(MCLOGQT_LIBRARY)
#  define MCLOGQT_EXPORT Q_DECL_EXPORT
# else
#  define MCLOGQT_EXPORT Q_DECL_IMPORT
# endif
#else
# define MCLOGQT_EXPORT
#endif

#include <QLoggingCategory>

#define MC_LOGGER(name) QLoggingCategory(name)

#define MC_DEFAULT_LOGGER MC_LOGGER("default")

#define MC_PRINT_ERR(...) \
    fprintf(stderr, __VA_ARGS__); \
    fflush(stderr);
