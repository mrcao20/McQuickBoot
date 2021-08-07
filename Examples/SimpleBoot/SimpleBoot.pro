QT += quick scxml

TARGET = SimpleBoot

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

STATECHARTS = BootTest.scxml

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

CONFIG(release, debug|release) {
    DEFINES += QT_MESSAGELOGCONTEXT
}

DESTDIR = $$PWD/../../bin/Examples
MOC_DIR = $$PWD/../../moc/Examples/SimpleBoot

include($$PWD/SimpleBoot.pri)
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

msvc {
    QMAKE_CFLAGS += /utf-8
    QMAKE_CXXFLAGS += /utf-8
}

QMAKE_CXXFLAGS_RELEASE = $$QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO
QMAKE_LFLAGS_RELEASE = $$QMAKE_LFLAGS_RELEASE_WITH_DEBUGINFO
