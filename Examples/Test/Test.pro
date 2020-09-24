QT += quick
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

SOURCES += \
        InvokeTest.cpp \
        IocTest.cpp \
        Object.cpp \
        QmlSocketTest.cpp \
        main.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DEFINES += QT_MESSAGELOGCONTEXT

DESTDIR = $$PWD/../../bin/Examples
MOC_DIR = $$PWD/../../moc/Examples/Test

DISTFILES += \
    myspring.xml \
    xmltest2.xml

HEADERS += \
    Interface.h \
    InvokeTest.h \
    IocTest.h \
    Object.h \
    QmlSocketTest.h \
    ThreadTest.h
    
include($$PWD/../../McIocBoot/McIocBootDepend.pri)

win32 {
    msvc {
        QMAKE_CFLAGS += /utf-8
        QMAKE_CXXFLAGS += /utf-8
        
        CONFIG(release, debug|release): LIBS += -L$$PWD/../../bin/ -lMcIocBoot
        else:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../bin/ -lMcIocBootd
    } else {
        equals(QT_MAJOR_VERSION, 5):lessThan(QT_MINOR_VERSION, 9) {
            CONFIG(release, debug|release): LIBS += -L$$PWD/../../bin/ -lMcIocBoot
            else:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../bin/ -lMcIocBootd
        } else {
            LIBS += -L$$PWD/../../bin/ -lMcIocBoot
        }
    }
} else:unix:!macx {
    LIBS += -L$$PWD/../../bin/ -lMcIocBoot
}

INCLUDEPATH += $$PWD/../../McIocBoot/include
DEPENDPATH += $$PWD/../../McIocBoot/include
