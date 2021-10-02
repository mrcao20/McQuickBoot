CONFIG += c++17 strict_c++ utf8_source warn_on

CONFIG(release, debug|release) {
    DEFINES += QT_MESSAGELOGCONTEXT
}

# 如果你在McIoc项目中声明此宏，那么你需要在进入main函数后，QCoreApplication构造之前手动调用Mc::Ioc::init来启动MC_STATIC的功能
# DEFINES += MC_MANUAL_ENABLE_IOC
# 因为quazip的开源协议为lgpl，暂时没法直接源码引入，所以定义此宏用于去掉该库，以后会考虑单独抽出McCore库来链接一些第三方库，并单独为其指定lgpl的开源协议
# DEFINES += MC_DISABLE_QUAZIP
# 添加QScxml的依赖
# DEFINES += MC_ENABLE_QSCXML

msvc {
    QMAKE_CFLAGS += /utf-8
    QMAKE_CXXFLAGS += /utf-8
}

QMAKE_CXXFLAGS_RELEASE = $$QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO
QMAKE_LFLAGS_RELEASE = $$QMAKE_LFLAGS_RELEASE_WITH_DEBUGINFO
