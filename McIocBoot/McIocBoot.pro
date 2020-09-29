#-------------------------------------------------
#
# Project created by QtCreator 2020-03-22T19:52:02
#
#-------------------------------------------------

QT += quick network

TARGET = McIocBoot
TARGET = $$qt5LibraryTarget($$TARGET)

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

include($$PWD/../common.pri)
include($$PWD/McIocBoot.pri)
include($$PWD/McIocBootDepend.pri)

unix {
    target.path = /usr/lib
    INSTALLS += target
}

DESTDIR = $$PWD/../bin
MOC_DIR = $$PWD/../moc/McIocBoot

INCLUDEPATH += $$PWD/../moc/McIocBoot
