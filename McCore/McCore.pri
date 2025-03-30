INCLUDEPATH += $$PWD

contains(DEFINES, MC_USE_LIBRARY) {
    DEFINES += MC_CORE_LIBRARY
}

HEADERS += \
    $$PWD/McMacroGlobal.h \
    $$PWD/McGlobal.h \
    $$PWD/McMetaType.h \
    $$PWD/Config/McConfigNode.h \
    $$PWD/Destroyer/IMcDestroyer.h \
    $$PWD/Event/McEventDispatcher.h \
    $$PWD/Event/McEventRouter.h \
    $$PWD/Utils/McJsonUtils.h \
    $$PWD/Utils/McSlotObjectWrapper.h \
    $$PWD/Utils/McCancel.h \
    $$PWD/Utils/McPause.h \
    $$PWD/Utils/McProgress.h \
    $$PWD/Utils/McRequest.h \
    $$PWD/Utils/McSignalBlocker.h \
    $$PWD/Utils/McCrashHandler.h \
    $$PWD/PluginChecker/IMcPluginChecker.h \
    $$PWD/PluginChecker/Impl/McNormalPluginChecker.h \
    $$PWD/MemoryLibrary/McMemoryLibrary.h \
    $$PWD/Callback/IMcCallback.h \
    $$PWD/Callback/Impl/McAbstractSyncCallback.h \
    $$PWD/Callback/Impl/McAbstractAsyncCallback.h \
    $$PWD/Callback/Impl/McCppSyncCallback.h \
    $$PWD/Callback/Impl/McCppAsyncCallback.h \
    $$PWD/Connection/McAbstractConnection.h \
    $$PWD/Connection/McCppConnection.h

SOURCES += \
    $$PWD/McGlobal.cpp \
    $$PWD/McMetaType.cpp \
    $$PWD/Config/McConfigNode.cpp \
    $$PWD/Event/McEventDispatcher.cpp \
    $$PWD/Event/McEventRouter.cpp \
    $$PWD/Utils/McSlotObjectWrapper.cpp \
    $$PWD/Utils/McJsonUtils.cpp \
    $$PWD/Utils/McCancel.cpp \
    $$PWD/Utils/McPause.cpp \
    $$PWD/Utils/McProgress.cpp \
    $$PWD/Utils/McRequest.cpp \
    $$PWD/Utils/McSignalBlocker.cpp \
    $$PWD/Utils/McCrashHandler.cpp \
    $$PWD/PluginChecker/Impl/McNormalPluginChecker.cpp \
    $$PWD/MemoryLibrary/McMemoryLibrary.cpp \
    $$PWD/Callback/Impl/McAbstractSyncCallback.cpp \
    $$PWD/Callback/Impl/McAbstractAsyncCallback.cpp \
    $$PWD/Callback/Impl/McCppSyncCallback.cpp \
    $$PWD/Callback/Impl/McCppAsyncCallback.cpp \
    $$PWD/Connection/McAbstractConnection.cpp \
    $$PWD/Connection/McCppConnection.cpp

!contains(DEFINES, MC_DISABLE_ZLIB) {
    HEADERS += $$PWD/Zip/McCompressor.h
    SOURCES += $$PWD/Zip/McCompressor.cpp
}

win32 {
    HEADERS += $$PWD/MemoryLibrary/MemoryModule.h
    SOURCES += $$PWD/MemoryLibrary/MemoryModule.c
} else {
    HEADERS += $$PWD/MemoryLibrary/memload.h
    SOURCES += $$PWD/MemoryLibrary/memload.cpp
}
