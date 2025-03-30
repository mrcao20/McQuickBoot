project(McCore LANGUAGES C CXX ASM)

configure_file(McConfig.h.in McConfig.h)

#include_directories(${Qt6Core_PRIVATE_INCLUDE_DIRS})

set(PROJECT_HEADERS
    McMacroGlobal.h
    McGlobal.h
    McMetaType.h
    Config/McConfigNode.h
    Destroyer/IMcDestroyer.h
    Event/McEventDispatcher.h
    Event/McEventRouter.h
    Utils/McJsonUtils.h
    Utils/McSlotObjectWrapper.h
    Utils/McCancel.h
    Utils/McPause.h
    Utils/McProgress.h
    Utils/McRequest.h
    Utils/McSignalBlocker.h
    Utils/McCrashHandler.h
    PluginChecker/IMcPluginChecker.h
    PluginChecker/Impl/McNormalPluginChecker.h
    MemoryLibrary/McMemoryLibrary.h
    Callback/IMcCallback.h
    Callback/Impl/McAbstractSyncCallback.h
    Callback/Impl/McAbstractAsyncCallback.h
    Callback/Impl/McCppSyncCallback.h
    Callback/Impl/McCppAsyncCallback.h
    Connection/McAbstractConnection.h
    Connection/McCppConnection.h
)

set(PROJECT_SOURCES
    McGlobal.cpp
    McMetaType.cpp
    Config/McConfigNode.cpp
    Event/McEventDispatcher.cpp
    Event/McEventRouter.cpp
    Utils/McSlotObjectWrapper.cpp
    Utils/McJsonUtils.cpp
    Utils/McCancel.cpp
    Utils/McPause.cpp
    Utils/McProgress.cpp
    Utils/McRequest.cpp
    Utils/McSignalBlocker.cpp
    Utils/McCrashHandler.cpp
    PluginChecker/Impl/McNormalPluginChecker.cpp
    MemoryLibrary/McMemoryLibrary.cpp
    Callback/Impl/McAbstractSyncCallback.cpp
    Callback/Impl/McAbstractAsyncCallback.cpp
    Callback/Impl/McCppSyncCallback.cpp
    Callback/Impl/McCppAsyncCallback.cpp
    Connection/McAbstractConnection.cpp
    Connection/McCppConnection.cpp
)

if(NOT DISABLE_ZLIB)
    list(APPEND PROJECT_HEADERS Zip/McCompressor.h)
    list(APPEND PROJECT_SOURCES Zip/McCompressor.cpp)
endif()

include(${CMAKE_SOURCE_DIR}/3rdparty/3rdparty.cmake)

if(WIN32)
    list(APPEND PROJECT_HEADERS MemoryLibrary/MemoryModule.h)
    list(APPEND PROJECT_SOURCES MemoryLibrary/MemoryModule.c)
else()
    list(APPEND PROJECT_HEADERS MemoryLibrary/memload.h)
    list(APPEND PROJECT_SOURCES MemoryLibrary/memload.cpp)
endif()

add_library(${PROJECT_NAME} ${MC_BUILD_TYPE}
    ${PROJECT_HEADERS}
    ${PROJECT_SOURCES}
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Core)
if(UNIX AND NOT ANDROID)
    target_link_libraries(${PROJECT_NAME} PRIVATE rt)
endif()

target_compile_definitions(${PROJECT_NAME} PRIVATE MC_CORE_LIBRARY)

include(${CMAKE_SOURCE_DIR}/cmake/Install.cmake)
foreach(hds IN LISTS MC_QUICKBOOT_INSTALL_HEADERS)
    install(FILES
        ${PROJECT_BINARY_DIR}/McConfig.h
        DESTINATION ${hds}/${PROJECT_NAME}
    )
endforeach()
