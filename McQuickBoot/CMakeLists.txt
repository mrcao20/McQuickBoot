project(McQuickBoot)

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_BINARY_DIR}/McCore)

set(PROJECT_HEADERS
    McQuickBootMacroGlobal.h
    McQuickBootGlobal.h
)

set(PROJECT_SOURCES
    McQuickBootGlobal.cpp
)

add_library(${PROJECT_NAME} ${MC_BUILD_TYPE}
    ${PROJECT_HEADERS}
    ${PROJECT_SOURCES}
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Core
    McCore McIoc)

target_compile_definitions(${PROJECT_NAME} PRIVATE MC_QUICKBOOT_LIBRARY)

include(${CMAKE_SOURCE_DIR}/cmake/Install.cmake)
