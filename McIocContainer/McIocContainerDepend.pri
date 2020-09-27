win32 {
    msvc {
        CONFIG(release, debug|release): LIBS += -L$$PWD/../bin/ -lMcYaml
        else:CONFIG(debug, debug|release): LIBS += -L$$PWD/../bin/ -lMcYamld
    } else {
        equals(QT_MAJOR_VERSION, 5):lessThan(QT_MINOR_VERSION, 9) {
            CONFIG(release, debug|release): LIBS += -L$$PWD/../bin/ -lMcYaml
            else:CONFIG(debug, debug|release): LIBS += -L$$PWD/../bin/ -lMcYamld
        } else {
            LIBS += -L$$PWD/../bin/ -lMcYaml
        }
    }
} else:unix:!macx {
    LIBS += -L$$PWD/../bin/ -lMcYaml
}

INCLUDEPATH += $$PWD/../McYaml/include
DEPENDPATH += $$PWD/../McYaml/include

!contains(DEFINES, MC_NO_YAML) {
    include($$PWD/../McYaml/McYamlDepend.pri)
}
