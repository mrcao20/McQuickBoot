#pragma once

#include <McIoc/McMacroGlobal.h>

#if defined(MC_BUILD_STATIC) && !defined(MC_EXPORT_DISABLE)
#define MC_EXPORT_DISABLE
#endif

#ifndef MC_EXPORT_DISABLE
#if defined(MCWIDGETIOC_LIBRARY)
#define MCWIDGETIOC_EXPORT Q_DECL_EXPORT
#else
#define MCWIDGETIOC_EXPORT Q_DECL_IMPORT
#endif
#else
#define MCWIDGETIOC_EXPORT
#endif
