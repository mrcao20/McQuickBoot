project(McCore)

configure_file(McConfig.h.in McConfig.h)

include_directories(${Qt6Core_PRIVATE_INCLUDE_DIRS})

set(PROJECT_HEADERS
    McMacroGlobal.h
    McGlobal.h
    McMetaType.h
    Config/McRuntimeConfig.h
    Config/McRuntimeConfigObject.h
    Config/McRuntimeConfigValue.h
    Event/McEventDispatcher.h
    Event/McEventRouter.h
    Utils/McQVariantConverter.h
    Utils/IMcDestroyer.h
    Utils/MemoryLibrary/McMemoryLibrary.h
)

set(PROJECT_SOURCES
    McGlobal.cpp
    McMetaType.cpp
    Config/McRuntimeConfig.cpp
    Config/McRuntimeConfigObject.cpp
    Config/McRuntimeConfigValue.cpp
    Event/McEventDispatcher.cpp
    Event/McEventRouter.cpp
    Utils/MemoryLibrary/McMemoryLibrary.cpp
)

if(WIN32)
    list(APPEND PROJECT_HEADERS Utils/MemoryLibrary/MemoryModule.h)
    list(APPEND PROJECT_SOURCES Utils/MemoryLibrary/MemoryModule.c)
else()
    list(APPEND PROJECT_HEADERS Utils/MemoryLibrary/memload.h)
    list(APPEND PROJECT_SOURCES Utils/MemoryLibrary/memload.cpp)
endif()

add_library(${PROJECT_NAME} ${MC_BUILD_TYPE}
    ${PROJECT_HEADERS}
    ${PROJECT_SOURCES}
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core)

target_compile_definitions(${PROJECT_NAME} PRIVATE MC_CORE_LIBRARY)

include(${CMAKE_SOURCE_DIR}/cmake/Install.cmake)
