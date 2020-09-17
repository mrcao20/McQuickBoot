#pragma once

#include <qglobal.h>

// version format MC_LOG_MAJOR.MC_LOG_MINOR.MC_LOG_PATCH.MC_LOG_INTERNAL
#define MC_LOG_MAJOR 1 // version when you make big feature changes.
#define MC_LOG_MINOR 0 // version when you make small feature changes.
#define MC_LOG_PATCH 0 // version when you make backwards-compatible bug fixes.
#define MC_LOG_INTERNAL 2 // MC_LOG_INTERNAL version for perpurse like feature test, bug fix test,development, et

#ifdef QT_DEBUG
#define MC_LOG_VERSION_STR (QString::number(MC_LOG_MAJOR) + "." + QString::number(MC_LOG_MINOR) + "." + QString::number(MC_LOG_PATCH) + "." + QString::number(MC_LOG_INTERNAL))
#define MC_LOG_VERSION ((MC_LOG_MAJOR<<24)|(MC_LOG_MINOR<<16)|(MC_LOG_PATCH<<8)|(MC_LOG_INTERNAL))
#else
#define MC_LOG_VERSION_STR (QString::number(MC_LOG_MAJOR) + "." + QString::number(MC_LOG_MINOR) + "." + QString::number(MC_LOG_PATCH))
#define MC_LOG_VERSION ((MC_LOG_MAJOR<<16)|(MC_LOG_MINOR<<8)|(MC_LOG_PATCH))
#endif
