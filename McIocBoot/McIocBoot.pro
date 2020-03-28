#-------------------------------------------------
#
# Project created by QtCreator 2020-03-22T19:52:02
#
#-------------------------------------------------

QT       += quick

QT       -= gui

CONFIG(debug, debug|release) {
    win32: TARGET = McIocBootd
    else:unix: TARGET = McIocBoot
} else {
    win32: TARGET = McIocBoot
    else:unix: TARGET = McIocBoot
}

TEMPLATE = lib

DEFINES += MCIOCBOOT_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/Controller/McControllerContainer.cpp \
    src/Controller/McQmlResponse.cpp \
    src/Controller/McRequestRunner.cpp \
    src/Controller/McResult.cpp \
    src/McIocBoot.cpp \
    src/Model/McModelContainer.cpp \
    src/Requestor/McQmlRequestor.cpp \
    src/Socket/McInnerSocket.cpp \
    src/Socket/McQmlSocket.cpp \
    src/Socket/McQmlSocketContainer.cpp \
    src/Socket/McQmlSocketRunner.cpp \
    src/Socket/McSession.cpp

HEADERS +=  \
    include/Controller/IMcControllerContainer.h \
    include/Controller/impl/McControllerContainer.h \
    include/Controller/impl/McQmlResponse.h \
    include/Controller/impl/McRequestRunner.h \
    include/Controller/impl/McResult.h \
    include/McBootGlobal.h \
    include/McBootMacroGlobal.h \
    include/McBootVersion.h \
    include/McIocBoot.h \
    include/Model/McModelContainer.h \
    include/Requestor/McQmlRequestor.h \
    include/Socket/IMcQmlSocketContainer.h \
    include/Socket/IMcSession.h \
    include/Socket/impl/McInnerSocket.h \
    include/Socket/impl/McQmlSocket.h \
    include/Socket/impl/McQmlSocketContainer.h \
    include/Socket/impl/McQmlSocketRunner.h \
    include/Socket/impl/McSession.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32: DESTDIR += $$PWD/../bin
else:unix: DESTDIR += $$PWD/../bin

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../bin/ -lMcIocContainer
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../bin/ -lMcIocContainerd

INCLUDEPATH += $$PWD/../McIocContainer/include
DEPENDPATH += $$PWD/../McIocContainer/include

RESOURCES += \
    qrc/Resource.qrc
