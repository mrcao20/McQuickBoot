win32 {
    msvc {
        win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../3rdparty/yaml-cpp/lib/lib/ -lyaml-cpp
        else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../3rdparty/yaml-cpp/lib/lib/ -lyaml-cppd
    } else {
        LIBS += $$PWD/../3rdparty/yaml-cpp/lib/bin/yaml-cpp.dll
    }
} else:unix:!macx {
    LIBS += -L$$PWD/../bin/ -lMcIocContainer
}

INCLUDEPATH += $$PWD/../3rdparty/yaml-cpp/lib/include
DEPENDPATH += $$PWD/../3rdparty/yaml-cpp/lib/include
