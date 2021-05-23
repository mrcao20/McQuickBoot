win32 {
    msvc {
        CONFIG(release, debug|release): LIBS += -L$$PWD/../bin/ -lMcWidgetIoc
        else:CONFIG(debug, debug|release): LIBS += -L$$PWD/../bin/ -lMcWidgetIocd
        CONFIG(release, debug|release): LIBS += -L$$PWD/../bin/ -lMcLogQt
        else:CONFIG(debug, debug|release): LIBS += -L$$PWD/../bin/ -lMcLogQtd
    } else {
        equals(QT_MAJOR_VERSION, 5):lessThan(QT_MINOR_VERSION, 13) {
            CONFIG(release, debug|release): LIBS += -L$$PWD/../bin/ -lMcWidgetIoc
            else:CONFIG(debug, debug|release): LIBS += -L$$PWD/../bin/ -lMcWidgetIocd
            CONFIG(release, debug|release): LIBS += -L$$PWD/../bin/ -lMcLogQt
            else:CONFIG(debug, debug|release): LIBS += -L$$PWD/../bin/ -lMcLogQtd
        } else {
            LIBS += -L$$PWD/../bin/ -lMcWidgetIoc
            LIBS += -L$$PWD/../bin/ -lMcLogQt
        }
    }
} else:unix:!macx {
    LIBS += -L$$PWD/../bin/ -lMcWidgetIoc
    LIBS += -L$$PWD/../bin/ -lMcLogQt
}

INCLUDEPATH += $$PWD/../McWidgetIoc/include
DEPENDPATH += $$PWD/../McWidgetIoc/include
INCLUDEPATH += $$PWD/../McLogQt/include
DEPENDPATH += $$PWD/../McLogQt/include

include($$PWD/../McWidgetIoc/McWidgetIocDepend.pri)
include($$PWD/../McLogQt/McLogQtDepend.pri)
