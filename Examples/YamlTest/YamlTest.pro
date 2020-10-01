#-------------------------------------------------
#
# Project created by QtCreator 2020-09-23T18:02:20
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = YamlTest
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        MainWindow.cpp

HEADERS  += MainWindow.h

FORMS    += MainWindow.ui

DESTDIR = $$PWD/../../bin/Examples
MOC_DIR = $$PWD/../../moc/Examples/YamlTest

include($$PWD/../../common.pri)
include($$PWD/../../McYaml/McYamlDepend.pri)

win32 {
    msvc {
        CONFIG(release, debug|release): LIBS += -L$$PWD/../../bin/ -lMcYaml
        else:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../bin/ -lMcYamld
    } else {
        equals(QT_MAJOR_VERSION, 5):lessThan(QT_MINOR_VERSION, 9) {
            CONFIG(release, debug|release): LIBS += -L$$PWD/../../bin/ -lMcYaml
            else:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../bin/ -lMcYamld
        } else {
            LIBS += -L$$PWD/../../bin/ -lMcYaml
        }
    }
} else:unix:!macx {
    LIBS += -L$$PWD/../../bin/ -lMcYaml
}

INCLUDEPATH += $$PWD/../../McYaml/include
DEPENDPATH += $$PWD/../../McYaml/include

# 将/替换为\\才能正确识别路径
SrcConfigPath = $$PWD/logqt.yml
SrcConfigPath = $$replace(SrcConfigPath, /, \\)
DstConfigPath = $$PWD/../../bin/Examples/
DstConfigPath = $$replace(DstConfigPath, /, \\)
QMAKE_POST_LINK += copy /y $$SrcConfigPath $$DstConfigPath

RESOURCES += \
    resource.qrc
