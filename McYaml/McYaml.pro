#-------------------------------------------------
#
# Project created by QtCreator 2020-09-23T17:46:52
#
#-------------------------------------------------

QT       -= gui

CONFIG += c++11

TARGET = McYaml
TARGET = $$qt5LibraryTarget($$TARGET)

CONFIG(release, debug|release) {
    DEFINES += QT_MESSAGELOGCONTEXT
}

TEMPLATE = lib

DEFINES += MCYAML_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

unix {
    target.path = /usr/lib
    INSTALLS += target
}

include(McYaml.pri)
include(McYamlDepend.pri)

DESTDIR = $$PWD/../bin
MOC_DIR = $$PWD/../moc/McYaml

msvc {
    QMAKE_CFLAGS += /utf-8
    QMAKE_CXXFLAGS += /utf-8
}

QMAKE_CXXFLAGS_RELEASE = $$QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO
QMAKE_LFLAGS_RELEASE = $$QMAKE_LFLAGS_RELEASE_WITH_DEBUGINFO
