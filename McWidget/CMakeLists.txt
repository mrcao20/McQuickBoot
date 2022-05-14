project(McWidget)

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_BINARY_DIR}/McCore)

set(PROJECT_HEADERS
)

set(PROJECT_SOURCES
)

add_library(${PROJECT_NAME} ${MC_BUILD_TYPE}
    ${PROJECT_HEADERS}
    ${PROJECT_SOURCES}
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core Qt6::Widgets
    McCore McIoc)

target_compile_definitions(${PROJECT_NAME} PRIVATE MC_WIDGET_LIBRARY)

include(${CMAKE_SOURCE_DIR}/cmake/Install.cmake)
