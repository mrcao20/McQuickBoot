#-------------------------------------------------
#
# Project created by QtCreator 2020-03-22T19:52:02
#
#-------------------------------------------------

QT += quick network

CONFIG(debug, debug|release) {
    win32: TARGET = McIocBootd
    else:unix: TARGET = McIocBoot
} else {
    win32: TARGET = McIocBoot
    else:unix: TARGET = McIocBoot

    DEFINES += QT_MESSAGELOGCONTEXT
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
    src/Application/McSingleApplication.cpp \
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
    src/Socket/McSession.cpp \
    src/Utils/McJsonUtils.cpp

HEADERS +=  \
    include/McBoot/Application/McSingleApplication.h \
    include/McBoot/Controller/IMcControllerContainer.h \
    include/McBoot/Controller/impl/McControllerContainer.h \
    include/McBoot/Controller/impl/McQmlResponse.h \
    include/McBoot/Controller/impl/McRequestRunner.h \
    include/McBoot/Controller/impl/McResult.h \
    include/McBoot/McBootGlobal.h \
    include/McBoot/McBootMacroGlobal.h \
    include/McBoot/McBootVersion.h \
    include/McBoot/McIocBoot.h \
    include/McBoot/Model/McModelContainer.h \
    include/McBoot/Requestor/McQmlRequestor.h \
    include/McBoot/Socket/IMcQmlSocketContainer.h \
    include/McBoot/Socket/IMcSession.h \
    include/McBoot/Socket/impl/McInnerSocket.h \
    include/McBoot/Socket/impl/McQmlSocket.h \
    include/McBoot/Socket/impl/McQmlSocketContainer.h \
    include/McBoot/Socket/impl/McQmlSocketRunner.h \
    include/McBoot/Socket/impl/McSession.h \
    include/McBoot/Utils/McJsonUtils.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

INCLUDEPATH += $$PWD/include/

DESTDIR = $$PWD/../bin
MOC_DIR = $$PWD/../moc/McIocBoot

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../bin/ -lMcIocContainer
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../bin/ -lMcIocContainerd

INCLUDEPATH += $$PWD/../McIocContainer/include
DEPENDPATH += $$PWD/../McIocContainer/include
