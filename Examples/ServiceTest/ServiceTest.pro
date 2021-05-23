QT -= gui

TEMPLATE = lib
DEFINES += SERVICETEST_LIBRARY

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

DESTDIR = $$PWD/../../bin/Examples/mcservices
MOC_DIR = $$PWD/../../moc/Examples/ServiceTest

include($$PWD/../../common.pri)
include($$PWD/../../McQuickBoot/McQuickBootDepend.pri)

win32 {
    msvc {
        CONFIG(release, debug|release): LIBS += -L$$PWD/../../bin/ -lMcQuickBoot
        else:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../bin/ -lMcQuickBootd
    } else {
        equals(QT_MAJOR_VERSION, 5):lessThan(QT_MINOR_VERSION, 13) {
            CONFIG(release, debug|release): LIBS += -L$$PWD/../../bin/ -lMcQuickBoot
            else:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../bin/ -lMcQuickBootd
        } else {
            LIBS += -L$$PWD/../../bin/ -lMcQuickBoot
        }
    }
} else:unix:!macx {
    LIBS += -L$$PWD/../../bin/ -lMcQuickBoot
}

INCLUDEPATH += $$PWD/../../McQuickBoot/include
DEPENDPATH += $$PWD/../../McQuickBoot/include

SOURCES += \
    Global.cpp

HEADERS +=

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
