QT.mcioc.VERSION = 5.15.0
QT.mcioc.name = McIoc
QT.mcioc.module = McIoc
QT.mcioc.libs = $$QT_MODULE_LIB_BASE
QT.mcioc.includes = $$QT_MODULE_INCLUDE_BASE $$QT_MODULE_INCLUDE_BASE/McIoc
QT.mcioc.bins = $$QT_MODULE_BIN_BASE
QT.mcioc.libexecs = $$QT_MODULE_LIBEXEC_BASE
QT.mcioc.imports = $$QT_MODULE_IMPORT_BASE
QT.mcioc.depends = core xml
QT.mcioc.DEFINES = MC_IOC_LIB
QT_MODULES += mcioc
