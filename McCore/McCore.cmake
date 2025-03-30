include_directories(${CMAKE_CURRENT_LIST_DIR})

if(USE_LIBRARY)
    add_definitions(-DMC_CORE_LIBRARY)
endif()

list(APPEND PROJECT_HEADERS
    ${CMAKE_CURRENT_LIST_DIR}/McMacroGlobal.h
    ${CMAKE_CURRENT_LIST_DIR}/McGlobal.h
    ${CMAKE_CURRENT_LIST_DIR}/McMetaType.h
    ${CMAKE_CURRENT_LIST_DIR}/Config/McConfigNode.h
    ${CMAKE_CURRENT_LIST_DIR}/Destroyer/IMcDestroyer.h
    ${CMAKE_CURRENT_LIST_DIR}/Event/McEventDispatcher.h
    ${CMAKE_CURRENT_LIST_DIR}/Event/McEventRouter.h
    ${CMAKE_CURRENT_LIST_DIR}/Utils/McJsonUtils.h
    ${CMAKE_CURRENT_LIST_DIR}/Utils/McSlotObjectWrapper.h
    ${CMAKE_CURRENT_LIST_DIR}/Utils/McCancel.h
    ${CMAKE_CURRENT_LIST_DIR}/Utils/McPause.h
    ${CMAKE_CURRENT_LIST_DIR}/Utils/McProgress.h
    ${CMAKE_CURRENT_LIST_DIR}/Utils/McRequest.h
    ${CMAKE_CURRENT_LIST_DIR}/PluginChecker/IMcPluginChecker.h
    ${CMAKE_CURRENT_LIST_DIR}/PluginChecker/Impl/McNormalPluginChecker.h
    ${CMAKE_CURRENT_LIST_DIR}/MemoryLibrary/McMemoryLibrary.h
    ${CMAKE_CURRENT_LIST_DIR}/Callback/IMcCallback.h
    ${CMAKE_CURRENT_LIST_DIR}/Callback/Impl/McAbstractSyncCallback.h
    ${CMAKE_CURRENT_LIST_DIR}/Callback/Impl/McAbstractAsyncCallback.h
    ${CMAKE_CURRENT_LIST_DIR}/Callback/Impl/McCppSyncCallback.h
    ${CMAKE_CURRENT_LIST_DIR}/Callback/Impl/McCppAsyncCallback.h
    ${CMAKE_CURRENT_LIST_DIR}/Connection/McAbstractConnection.h
    ${CMAKE_CURRENT_LIST_DIR}/Connection/McCppConnection.h
    ${CMAKE_CURRENT_LIST_DIR}/Zip/McCompressor.h
)

list(APPEND PROJECT_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/McGlobal.cpp
    ${CMAKE_CURRENT_LIST_DIR}/McMetaType.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Config/McConfigNode.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Event/McEventDispatcher.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Event/McEventRouter.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Utils/McSlotObjectWrapper.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Utils/McJsonUtils.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Utils/McCancel.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Utils/McPause.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Utils/McProgress.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Utils/McRequest.cpp
    ${CMAKE_CURRENT_LIST_DIR}/PluginChecker/Impl/McNormalPluginChecker.cpp
    ${CMAKE_CURRENT_LIST_DIR}/MemoryLibrary/McMemoryLibrary.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Callback/Impl/McAbstractSyncCallback.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Callback/Impl/McAbstractAsyncCallback.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Callback/Impl/McCppSyncCallback.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Callback/Impl/McCppAsyncCallback.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Connection/McAbstractConnection.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Connection/McCppConnection.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Zip/McCompressor.cpp
)

include(${CMAKE_CURRENT_LIST_DIR}/../3rdparty/3rdparty.cmake)

if(WIN32)
    list(APPEND PROJECT_HEADERS ${CMAKE_CURRENT_LIST_DIR}/MemoryLibrary/MemoryModule.h)
    list(APPEND PROJECT_SOURCES ${CMAKE_CURRENT_LIST_DIR}/MemoryLibrary/MemoryModule.c)
else()
    list(APPEND PROJECT_HEADERS ${CMAKE_CURRENT_LIST_DIR}/MemoryLibrary/memload.h)
    list(APPEND PROJECT_SOURCES ${CMAKE_CURRENT_LIST_DIR}/MemoryLibrary/memload.cpp)
endif()
