#pragma once

#include <qglobal.h>

// version format MC_ORM_MAJOR.MC_ORM_MINOR.MC_ORM_PATCH.MC_ORM_INTERNAL
#define MC_ORM_MAJOR 1 // version when you make big feature changes.
#define MC_ORM_MINOR 0 // version when you make small feature changes.
#define MC_ORM_PATCH 0 // version when you make backwards-compatible bug fixes.
#define MC_ORM_INTERNAL 0 // MC_ORM_INTERNAL version for perpurse like feature test, bug fix test,development, et

#ifdef QT_DEBUG
#define MC_ORM_VERSION_STR (QString::number(MC_ORM_MAJOR) + "." + QString::number(MC_ORM_MINOR) + "." + QString::number(MC_ORM_PATCH) + "." + QString::number(MC_ORM_INTERNAL))
#define MC_ORM_VERSION ((MC_ORM_MAJOR<<24)|(MC_ORM_MINOR<<16)|(MC_ORM_PATCH<<8)|(MC_ORM_INTERNAL))
#else
#define MC_ORM_VERSION_STR (QString::number(MC_ORM_MAJOR) + "." + QString::number(MC_ORM_MINOR) + "." + QString::number(MC_ORM_PATCH))
#define MC_ORM_VERSION ((MC_ORM_MAJOR<<16)|(MC_ORM_MINOR<<8)|(MC_ORM_PATCH))
#endif
