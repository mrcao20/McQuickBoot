INCLUDEPATH += $$PWD

contains(DEFINES, MC_USE_LIBRARY) {
    DEFINES += MC_QUICKBOOT_LIBRARY
}

HEADERS += \
    $$PWD/McQuickBootMacroGlobal.h \
    $$PWD/McQuickBootGlobal.h \
    $$PWD/Invoker/McInvoker.h \
    $$PWD/Invoker/McAbstractPromise.h \
    $$PWD/Invoker/McCppPromise.h \
    $$PWD/Invoker/McResult.h

SOURCES += \
    $$PWD/McQuickBootGlobal.cpp \
    $$PWD/Invoker/McInvoker.cpp \
    $$PWD/Invoker/McAbstractPromise.cpp \
    $$PWD/Invoker/McCppPromise.cpp \
    $$PWD/Invoker/McResult.cpp
