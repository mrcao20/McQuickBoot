#-------------------------------------------------
#
# Project created by QtCreator 2020-09-23T17:46:52
#
#-------------------------------------------------

QT       -= gui

TARGET = McYaml
TARGET = $$qt5LibraryTarget($$TARGET)

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

include($$PWD/../common.pri)
include($$PWD/McYaml.pri)
include($$PWD/McYamlDepend.pri)

DESTDIR = $$PWD/../bin
MOC_DIR = $$PWD/../moc/McYaml
