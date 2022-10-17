INCLUDEPATH += $$PWD

contains(DEFINES, MC_USE_LIBRARY) {
    DEFINES += MC_QUICKBOOT_LIBRARY
}

HEADERS += \
    $$PWD/McQuickBootMacroGlobal.h \
    $$PWD/McQuickBootGlobal.h

SOURCES += \
    $$PWD/McQuickBootGlobal.cpp
