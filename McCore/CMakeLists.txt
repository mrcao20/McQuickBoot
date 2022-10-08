project(McCore)

configure_file(McConfig.h.in McConfig.h)

#include_directories(${Qt6Core_PRIVATE_INCLUDE_DIRS})

set(PROJECT_HEADERS
    McMacroGlobal.h
    McGlobal.h
    McMetaType.h
    Config/McRuntimeConfig.h
    Config/McRuntimeConfigObject.h
    Config/McRuntimeConfigValue.h
    Destroyer/IMcDestroyer.h
    Event/McEventDispatcher.h
    Event/McEventRouter.h
    Utils/McJsonUtils.h
    Utils/McSlotObjectWrapper.h
    Utils/McCancel.h
    Utils/McPause.h
    Utils/McProgress.h
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
    Config/McRuntimeConfig.cpp
    Config/McRuntimeConfigObject.cpp
    Config/McRuntimeConfigValue.cpp
    Event/McEventDispatcher.cpp
    Event/McEventRouter.cpp
    Utils/McSlotObjectWrapper.cpp
    Utils/McJsonUtils.cpp
    Utils/McCancel.cpp
    Utils/McPause.cpp
    Utils/McProgress.cpp
    PluginChecker/Impl/McNormalPluginChecker.cpp
    MemoryLibrary/McMemoryLibrary.cpp
    Callback/Impl/McAbstractSyncCallback.cpp
    Callback/Impl/McAbstractAsyncCallback.cpp
    Callback/Impl/McCppSyncCallback.cpp
    Callback/Impl/McCppAsyncCallback.cpp
    Connection/McAbstractConnection.cpp
    Connection/McCppConnection.cpp
)

if(QT_VERSION_MAJOR EQUAL 5)
    list(APPEND PROJECT_HEADERS
        Utils/McJsonUtils_Qt5.h
    )

    list(APPEND PROJECT_SOURCES
        Utils/McJsonUtils_Qt5.cpp
    )
endif()

if(WIN32)
    list(APPEND PROJECT_HEADERS
        MemoryLibrary/MemoryModule.h
        Zip/McCompressor.h
        Zip/zip.h
        Zip/unzip.h
        Zip/ioapi.h
        Zip/crypt.h
    )

    list(APPEND PROJECT_SOURCES
        MemoryLibrary/MemoryModule.c
        Zip/McCompressor.cpp
        Zip/zip.c
        Zip/unzip.c
        Zip/ioapi.c
    )
else()
    list(APPEND PROJECT_HEADERS MemoryLibrary/memload.h)
    list(APPEND PROJECT_SOURCES MemoryLibrary/memload.cpp)
endif()

add_library(${PROJECT_NAME} ${MC_BUILD_TYPE}
    ${PROJECT_HEADERS}
    ${PROJECT_SOURCES}
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Core)

target_compile_definitions(${PROJECT_NAME} PRIVATE MC_CORE_LIBRARY)

include(${CMAKE_SOURCE_DIR}/cmake/Install.cmake)
foreach(hds IN LISTS MC_QUICKBOOT_INSTALL_HEADERS)
    install(FILES
        ${PROJECT_BINARY_DIR}/McConfig.h
        DESTINATION ${hds}/${PROJECT_NAME}
    )
endforeach()
