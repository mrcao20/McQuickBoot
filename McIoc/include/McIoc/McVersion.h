#pragma once

#include <qglobal.h>

//! version format MC_MAJOR.MC_MINOR.MC_PATCH.MC_INTERNAL
//! version when you make big feature changes.
#define MC_MAJOR 1
//! version when you make small feature changes.
#define MC_MINOR 5
//! version when you make backwards-compatible bug fixes.
#define MC_PATCH 0
//! MC_INTERNAL version for perpurse like feature test, bug fix test,development, et
#define MC_INTERNAL 7

#ifdef QT_DEBUG
#define MC_VERSION_STR (QString::number(MC_MAJOR) + "." + QString::number(MC_MINOR) + "." + QString::number(MC_PATCH) + "." + QString::number(MC_INTERNAL))
#define MC_VERSION ((MC_MAJOR<<24)|(MC_MINOR<<16)|(MC_PATCH<<8)|(MC_INTERNAL))
#else
#define MC_VERSION_STR (QString::number(MC_MAJOR) + "." + QString::number(MC_MINOR) + "." + QString::number(MC_PATCH))
#define MC_VERSION ((MC_MAJOR<<16)|(MC_MINOR<<8)|(MC_PATCH))
#endif
