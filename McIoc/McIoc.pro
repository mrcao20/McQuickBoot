#-------------------------------------------------
#
# Project created by QtCreator 2020-03-15T21:42:44
#
#-------------------------------------------------

QT -= gui

TARGET = McIoc
TARGET = $$qt5LibraryTarget($$TARGET)

TEMPLATE = lib

DEFINES += MCIOC_LIBRARY

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
include($$PWD/McIoc.pri)
include($$PWD/McIocDepend.pri)

DESTDIR = $$PWD/../bin
MOC_DIR = $$PWD/../moc/McIoc

unix {
    target.path = /usr/lib
    INSTALLS += target
} else:win32 {
    include_target.path = $$[QT_INSTALL_HEADERS]
    include_target.files = $$PWD/include/*
    
    bin_target.path = $$[QT_INSTALL_BINS]
    bin_target.files = $$DESTDIR/$${TARGET}.dll
    
    lib_target.path = $$[QT_INSTALL_LIBS]
    lib_target.files = $$DESTDIR/$${TARGET}.lib $$DESTDIR/$${TARGET}.pdb
    
    module_target.path = $$[QT_INSTALL_PREFIX]/mkspecs/modules
    module_target.files = $$PWD/../modules/qt_lib_ioc.pri
    
    INSTALLS += include_target bin_target lib_target module_target
}
