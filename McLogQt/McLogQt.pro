#-------------------------------------------------
#
# Project created by QtCreator 2020-03-28T20:02:33
#
#-------------------------------------------------

QT       -= gui

TARGET = McLogQt
TARGET = $$qt5LibraryTarget($$TARGET)

CONFIG(release, debug|release) {
    DEFINES += QT_MESSAGELOGCONTEXT
}

TEMPLATE = lib

DEFINES += MCLOGQT_LIBRARY

# 去掉IOC容器的依赖部分
#DEFINES += MC_NO_IOC

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

include(McLogQt.pri)

DESTDIR = $$PWD/../bin
MOC_DIR = $$PWD/../moc/McLogQt

contains(DEFINES, MC_NO_IOC) {
    message(defined MC_NO_IOC)
}else{
    win32 {
        msvc {
            QMAKE_CFLAGS += /utf-8
            QMAKE_CXXFLAGS += /utf-8
            
            CONFIG(release, debug|release): LIBS += -L$$PWD/../bin/ -lMcIocContainer
            else:CONFIG(debug, debug|release): LIBS += -L$$PWD/../bin/ -lMcIocContainerd
        } else {
            LIBS += -L$$PWD/../bin/ -lMcIocContainer
        }
    } else:unix:!macx {
        LIBS += -L$$PWD/../bin/ -lMcIocContainer
    }
    
    INCLUDEPATH += $$PWD/../McIocContainer/include
    DEPENDPATH += $$PWD/../McIocContainer/include
}
