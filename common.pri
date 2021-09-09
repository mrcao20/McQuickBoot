CONFIG += c++17 strict_c++ utf8_source warn_on

CONFIG(release, debug|release) {
    DEFINES += QT_MESSAGELOGCONTEXT
}

# 如果你在McIoc项目中声明此宏，那么你需要在进入main函数后，QCoreApplication构造之前手动调用Mc::Ioc::init来启动MC_STATIC的功能
# DEFINES += MC_MANUAL_ENABLE_IOC

msvc {
    QMAKE_CFLAGS += /utf-8
    QMAKE_CXXFLAGS += /utf-8
}

QMAKE_CXXFLAGS_RELEASE = $$QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO
QMAKE_LFLAGS_RELEASE = $$QMAKE_LFLAGS_RELEASE_WITH_DEBUGINFO
