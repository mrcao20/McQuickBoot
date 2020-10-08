win32 {
    msvc {
        CONFIG(release, debug|release): LIBS += -L$$PWD/../bin/ -lMcIocContainer
        else:CONFIG(debug, debug|release): LIBS += -L$$PWD/../bin/ -lMcIocContainerd
    } else {
        equals(QT_MAJOR_VERSION, 5):lessThan(QT_MINOR_VERSION, 9) {
            CONFIG(release, debug|release): LIBS += -L$$PWD/../bin/ -lMcIocContainer
            else:CONFIG(debug, debug|release): LIBS += -L$$PWD/../bin/ -lMcIocContainerd
        } else {
            LIBS += -L$$PWD/../bin/ -lMcIocContainer
        }
    }
} else:unix:!macx {
    LIBS += -L$$PWD/../bin/ -lMcIocContainer
}

INCLUDEPATH += $$PWD/../McIocContainer/include
DEPENDPATH += $$PWD/../McIocContainer/include

include($$PWD/../McIocContainer/McIocContainerDepend.pri)
