QT += qml quick

CONFIG += c++11

SOURCES += main.cpp \
    InvokeTest.cpp \
    QmlSocketTest.cpp \
    Object.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DESTDIR = $$PWD/../../bin/Examples
MOC_DIR = $$PWD/../../moc/Examples/BootTest

include($$PWD/../../common.pri)
include($$PWD/../../McIocBoot/McIocBootDepend.pri)

win32 {
    msvc {
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

# 将/替换为\\才能正确识别路径
SrcConfigPath = $$PWD/application.yml
SrcConfigPath = $$replace(SrcConfigPath, /, \\)
DstConfigPath = $$PWD/../../bin/Examples/
DstConfigPath = $$replace(DstConfigPath, /, \\)
QMAKE_POST_LINK += copy /y $$SrcConfigPath $$DstConfigPath

HEADERS += \
    InvokeTest.h \
    QmlSocketTest.h \
    Object.h
