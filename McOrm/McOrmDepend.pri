win32 {
    msvc {
        CONFIG(release, debug|release): LIBS += -L$$PWD/../bin/ -lMcIoc
        else:CONFIG(debug, debug|release): LIBS += -L$$PWD/../bin/ -lMcIocd
    } else {
        equals(QT_MAJOR_VERSION, 5):lessThan(QT_MINOR_VERSION, 13) {
            CONFIG(release, debug|release): LIBS += -L$$PWD/../bin/ -lMcIoc
            else:CONFIG(debug, debug|release): LIBS += -L$$PWD/../bin/ -lMcIocd
        } else {
            LIBS += -L$$PWD/../bin/ -lMcIoc
        }
    }
} else:unix:!macx {
    LIBS += -L$$PWD/../bin/ -lMcIoc
}

INCLUDEPATH += $$PWD/../McIoc/include
DEPENDPATH += $$PWD/../McIoc/include

include($$PWD/../McIoc/McIocDepend.pri)
