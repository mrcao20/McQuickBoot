project(Tests)

find_package(Qt6 COMPONENTS Test REQUIRED)

set(CMAKE_DEBUG_POSTFIX "")

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_BINARY_DIR}/McCore)

add_subdirectory(SimpleLibrary)
add_subdirectory(SimplePlugin)
add_subdirectory(CoreTest)
add_subdirectory(IocTest)
