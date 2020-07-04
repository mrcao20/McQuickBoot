#pragma once

#include <qglobal.h>

// version format MC_IOC_MAJOR.MC_IOC_MINOR.MC_IOC_PATCH.MC_IOC_INTERNAL
#define MC_IOC_MAJOR 1 // version when you make big feature changes.
#define MC_IOC_MINOR 2 // version when you make small feature changes.
#define MC_IOC_PATCH 0 // version when you make backwards-compatible bug fixes.
#define MC_IOC_INTERNAL 0 // MC_IOC_INTERNAL version for perpurse like feature test, bug fix test,development, et

#ifdef QT_DEBUG
#define MC_IOC_VERSION_STR (QString::number(MC_IOC_MAJOR) + "." + QString::number(MC_IOC_MINOR) + "." + QString::number(MC_IOC_PATCH) + "." + QString::number(MC_IOC_INTERNAL))
#define MC_IOC_VERSION ((MC_IOC_MAJOR<<24)|(MC_IOC_MINOR<<16)|(MC_IOC_PATCH<<8)|(MC_IOC_INTERNAL))
#else
#define MC_IOC_VERSION_STR (QString::number(MC_IOC_MAJOR) + "." + QString::number(MC_IOC_MINOR) + "." + QString::number(MC_IOC_PATCH))
#define MC_IOC_VERSION ((MC_IOC_MAJOR<<16)|(MC_IOC_MINOR<<8)|(MC_IOC_PATCH))
#endif
