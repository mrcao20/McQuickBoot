project(Examples)

find_package(Qt6 COMPONENTS Widgets REQUIRED)

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_BINARY_DIR}/McCore)

add_subdirectory(CoreExample)
