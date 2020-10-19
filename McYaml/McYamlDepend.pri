win32 {
    msvc {
        win32: LIBS += -L$$PWD/../3rdparty/yaml-cpp/lib/lib/ -lyaml-cpp
    } else {
        LIBS += $$PWD/../3rdparty/yaml-cpp/lib/bin/yaml-cpp.dll
    }
} else:unix:!macx {
    LIBS += -L$$PWD/../bin/ -McIoc
}

INCLUDEPATH += $$PWD/../3rdparty/yaml-cpp/lib/include
DEPENDPATH += $$PWD/../3rdparty/yaml-cpp/lib/include
