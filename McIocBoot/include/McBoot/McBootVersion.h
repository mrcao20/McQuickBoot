#pragma once

#include <qglobal.h>

// version format MC_BOOT_MAJOR.MC_BOOT_MINOR.MC_BOOT_PATCH.MC_BOOT_INTERNAL
#define MC_BOOT_MAJOR 1 // version when you make big feature changes.
#define MC_BOOT_MINOR 3 // version when you make small feature changes.
#define MC_BOOT_PATCH 0 // version when you make backwards-compatible bug fixes.
#define MC_BOOT_INTERNAL 2 // MC_BOOT_INTERNAL version for perpurse like feature test, bug fix test,development, et

#ifdef QT_DEBUG
#define MC_BOOT_VERSION_STR (QString::number(MC_BOOT_MAJOR) + "." + QString::number(MC_BOOT_MINOR) + "." + QString::number(MC_BOOT_PATCH) + "." + QString::number(MC_BOOT_INTERNAL))
#define MC_BOOT_VERSION ((MC_BOOT_MAJOR<<24)|(MC_BOOT_MINOR<<16)|(MC_BOOT_PATCH<<8)|(MC_BOOT_INTERNAL))
#else
#define MC_BOOT_VERSION_STR (QString::number(MC_BOOT_MAJOR) + "." + QString::number(MC_BOOT_MINOR) + "." + QString::number(MC_BOOT_PATCH))
#define MC_BOOT_VERSION ((MC_BOOT_MAJOR<<16)|(MC_BOOT_MINOR<<8)|(MC_BOOT_PATCH))
#endif
