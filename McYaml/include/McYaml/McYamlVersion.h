#pragma once

#include <qglobal.h>

// version format MC_YAML_MAJOR.MC_YAML_MINOR.MC_YAML_PATCH.MC_YAML_INTERNAL
#define MC_YAML_MAJOR 1 // version when you make big feature changes.
#define MC_YAML_MINOR 0 // version when you make small feature changes.
#define MC_YAML_PATCH 0 // version when you make backwards-compatible bug fixes.
#define MC_YAML_INTERNAL 0 // MC_YAML_INTERNAL version for perpurse like feature test, bug fix test,development, et

#ifdef QT_DEBUG
#define MC_YAML_VERSION_STR (QString::number(MC_YAML_MAJOR) + "." + QString::number(MC_YAML_MINOR) + "." + QString::number(MC_YAML_PATCH) + "." + QString::number(MC_YAML_INTERNAL))
#define MC_YAML_VERSION ((MC_YAML_MAJOR<<24)|(MC_YAML_MINOR<<16)|(MC_YAML_PATCH<<8)|(MC_YAML_INTERNAL))
#else
#define MC_YAML_VERSION_STR (QString::number(MC_YAML_MAJOR) + "." + QString::number(MC_YAML_MINOR) + "." + QString::number(MC_YAML_PATCH))
#define MC_YAML_VERSION ((MC_YAML_MAJOR<<16)|(MC_YAML_MINOR<<8)|(MC_YAML_PATCH))
#endif
