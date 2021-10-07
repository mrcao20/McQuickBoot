QT += quick widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DESTDIR = $$PWD/../../bin/Examples
MOC_DIR = $$PWD/../../moc/Examples/QuickBootExample

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

SrcConfigPath = $$PWD/application.yml
DstConfigPath = $$PWD/../../bin/Examples/
win32 {
    SrcConfigPath = $$replace(SrcConfigPath, /, \\)
    DstConfigPath = $$replace(DstConfigPath, /, \\)
    QMAKE_POST_LINK += copy /y $$SrcConfigPath $$DstConfigPath
} else {
    QMAKE_POST_LINK += cp $$SrcConfigPath $$DstConfigPath
}

SOURCES += \
        Controller/Controller2.cpp \
        Domain/DomainGlobal.cpp \
        Service/Service2.cpp \
        Widget/ChildWidget1.cpp \
        Widget/ChildWidget2.cpp \
        Widget/ChildWidget3.cpp \
        Widget/MainWindow.cpp \
        main.cpp

RESOURCES += qml.qrc

OTHER_FILES += \
    $$PWD/application.yml \
    widgetioc.xml

FORMS +=

HEADERS += \
    Controller/Controller2.h \
    Domain/RequestVo.h \
    Domain/ReturnVo.h \
    Service/Service2.h \
    Widget/ChildWidget1.h \
    Widget/ChildWidget2.h \
    Widget/ChildWidget3.h \
    Widget/MainWindow.h
