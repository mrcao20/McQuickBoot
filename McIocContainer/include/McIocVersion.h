#pragma once

#include <qglobal.h>

// version format MAJOR.MINOR.PATCH.INTERNAL
#define MAJOR 1 // version when you make big feature changes.
#define MINOR 0 // version when you make small feature changes.
#define PATCH 0 // version when you make backwards-compatible bug fixes.
#define INTERNAL 0 // internal version for perpurse like feature test, bug fix test,development, et

#ifdef QT_DEBUG
#define VERSION_STR (QString::number(MAJOR) + "." + QString::number(MINOR) + "." + QString::number(PATCH) + "." + QString::number(INTERNAL))
#define VERSION ((MAJOR<<24)|(MINOR<<16)|(PATCH<<8)|(INTERNAL))
#else
#define VERSION_STR (QString::number(MAJOR) + "." + QString::number(MINOR) + "." + QString::number(PATCH))
#define VERSION ((MAJOR<<16)|(MINOR<<8)|(PATCH))
#endif
