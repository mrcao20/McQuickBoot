QT.quickboot.VERSION = 5.15.0
QT.quickboot.name = McQuickBoot
QT.quickboot.module = McQuickBoot
QT.quickboot.libs = $$QT_MODULE_LIB_BASE
QT.quickboot.includes = $$QT_MODULE_INCLUDE_BASE $$QT_MODULE_INCLUDE_BASE/McQuickBoot
QT.quickboot.bins = $$QT_MODULE_BIN_BASE
QT.quickboot.libexecs = $$QT_MODULE_LIBEXEC_BASE
QT.quickboot.imports = $$QT_MODULE_IMPORT_BASE
QT.quickboot.depends = quick quickwidgets ioc logqt
QT.quickboot.DEFINES = MC_QUICKBOOT_LIB
QT_MODULES += quickboot
