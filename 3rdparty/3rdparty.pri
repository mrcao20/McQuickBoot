INCLUDEPATH += $$PWD

!contains(DEFINES, MC_DISABLE_ZLIB) {
    include(zlib/zlib.pri)
}
include(yaml-cpp/yaml-cpp.pri)
include(google_breakpad/google_breakpad.pri)
