#-------------------------------------------------
#
# Project created by QtCreator 2020-03-22T19:52:02
#
#-------------------------------------------------

QT       += quick

QT       -= gui

CONFIG(debug, debug|release) {
    win32: TARGET = McIocBootd
    else:unix: TARGET = McIocBoot
} else {
    win32: TARGET = McIocBoot
    else:unix: TARGET = McIocBoot
}

TEMPLATE = lib

DEFINES += MCIOCBOOT_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES +=

HEADERS +=  \
    include/McBootGlobal.h \
    include/McBootMacroGlobal.h \
    include/McBootVersion.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32: DESTDIR += $$PWD/../bin
else:unix: DESTDIR += $$PWD/../bin

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../bin/ -lMcIocContainer
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../bin/ -lMcIocContainerd

INCLUDEPATH += $$PWD/../McIocContainer/include
DEPENDPATH += $$PWD/../McIocContainer/include
