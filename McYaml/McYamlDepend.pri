win32 {
    msvc {
        QMAKE_CFLAGS += /utf-8
        QMAKE_CXXFLAGS += /utf-8
        
        win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../3rdparty/yaml-cpp/lib/lib/ -lyaml-cpp
        else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../3rdparty/yaml-cpp/lib/lib/ -lyaml-cppd
    } else {
        equals(QT_MAJOR_VERSION, 5):lessThan(QT_MINOR_VERSION, 9) {
            win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../3rdparty/yaml-cpp/lib/lib/ -lyaml-cpp
            else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../3rdparty/yaml-cpp/lib/lib/ -lyaml-cppd
        } else {
            LIBS += -L$$PWD/../3rdparty/yaml-cpp/lib/lib/ -lyaml-cpp
        }
    }
} else:unix:!macx {
    LIBS += -L$$PWD/../bin/ -lMcIocContainer
}

INCLUDEPATH += $$PWD/../3rdparty/yaml-cpp/lib/include
DEPENDPATH += $$PWD/../3rdparty/yaml-cpp/lib/include
