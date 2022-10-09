DEFINES += MC_CORE_LIBRARY
HEADERS += \
    $$PWD/McMacroGlobal.h \
    $$PWD/McGlobal.h \
    $$PWD/McMetaType.h \
    $$PWD/Config/McRuntimeConfig.h \
    $$PWD/Config/McRuntimeConfigObject.h \
    $$PWD/Config/McRuntimeConfigValue.h \
    $$PWD/Destroyer/IMcDestroyer.h \
    $$PWD/Event/McEventDispatcher.h \
    $$PWD/Event/McEventRouter.h \
    $$PWD/Utils/McJsonUtils.h \
    $$PWD/Utils/McSlotObjectWrapper.h \
    $$PWD/Utils/McCancel.h \
    $$PWD/Utils/McPause.h \
    $$PWD/Utils/McProgress.h \
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
    $$PWD/Config/McRuntimeConfig.cpp \
    $$PWD/Config/McRuntimeConfigObject.cpp \
    $$PWD/Config/McRuntimeConfigValue.cpp \
    $$PWD/Event/McEventDispatcher.cpp \
    $$PWD/Event/McEventRouter.cpp \
    $$PWD/Utils/McSlotObjectWrapper.cpp \
    $$PWD/Utils/McJsonUtils.cpp \
    $$PWD/Utils/McCancel.cpp \
    $$PWD/Utils/McPause.cpp \
    $$PWD/Utils/McProgress.cpp \
    $$PWD/PluginChecker/Impl/McNormalPluginChecker.cpp \
    $$PWD/MemoryLibrary/McMemoryLibrary.cpp \
    $$PWD/Callback/Impl/McAbstractSyncCallback.cpp \
    $$PWD/Callback/Impl/McAbstractAsyncCallback.cpp \
    $$PWD/Callback/Impl/McCppSyncCallback.cpp \
    $$PWD/Callback/Impl/McCppAsyncCallback.cpp \
    $$PWD/Connection/McAbstractConnection.cpp \
    $$PWD/Connection/McCppConnection.cpp

win32 {
    HEADERS += \
        $$PWD/MemoryLibrary/MemoryModule.h \
        $$PWD/Zip/McCompressor.h \
        $$PWD/Zip/zip.h \
        $$PWD/Zip/unzip.h \
        $$PWD/Zip/ioapi.h \
        $$PWD/Zip/crypt.h

    SOURCES += \
        $$PWD/MemoryLibrary/MemoryModule.c \
        $$PWD/Zip/McCompressor.cpp \
        $$PWD/Zip/zip.c \
        $$PWD/Zip/unzip.c \
        $$PWD/Zip/ioapi.c
} else {
    HEADERS += $$PWD/MemoryLibrary/memload.h
    SOURCES += $$PWD/MemoryLibrary/memload.cpp
}
