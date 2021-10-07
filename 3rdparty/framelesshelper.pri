QT *= gui-private

DEFINES += FRAMELESSHELPER_STATIC

INCLUDEPATH += $$PWD/framelesshelper/

HEADERS += \
    $$PWD/framelesshelper/framelesshelper_global.h \
    $$PWD/framelesshelper/framelesshelper.h \
    $$PWD/framelesshelper/framelesswindowsmanager.h \
    $$PWD/framelesshelper/utilities.h
SOURCES += \
    $$PWD/framelesshelper/framelesshelper.cpp \
    $$PWD/framelesshelper/framelesswindowsmanager.cpp \
    $$PWD/framelesshelper/utilities.cpp
qtHaveModule(quick) {
    QT *= quick
    HEADERS += $$PWD/framelesshelper/framelessquickhelper.h
    SOURCES += $$PWD/framelesshelper/framelessquickhelper.cpp
}
win32 {
    HEADERS += \
        $$PWD/framelesshelper/framelesshelper_windows.h
        $$PWD/framelesshelper/framelesshelper_win32.h
    SOURCES += \
        $$PWD/framelesshelper/utilities_win32.cpp \
        $$PWD/framelesshelper/framelesshelper_win32.cpp
    LIBS += -luser32 -lshell32 -ldwmapi
    RC_FILE = $$PWD/framelesshelper/framelesshelper.rc
} else:unix:!macx {
    SOURCES += $$PWD/framelesshelper/utilities_linux.cpp
} else {
    SOURCES += $$PWD/framelesshelper/utilities_macos.mm
}
