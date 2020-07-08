#-------------------------------------------------
#
# Project created by QtCreator 2020-03-15T21:42:44
#
#-------------------------------------------------

QT -= gui
QT += xml

CONFIG(debug, debug|release) {
    win32: TARGET = McIocContainerd
    else:unix: TARGET = McIocContainer
} else {
    win32: TARGET = McIocContainer
    else:unix: TARGET = McIocContainer

    DEFINES += QT_MESSAGELOGCONTEXT
}


TEMPLATE = lib

DEFINES += MCIOCCONTAINER_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
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

include(McIocContainer.pri)

DESTDIR = $$PWD/../bin
MOC_DIR = $$PWD/../moc/McIocContainer

msvc {
    QMAKE_CFLAGS += /utf-8
    QMAKE_CXXFLAGS += /utf-8
}
