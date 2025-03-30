project(Tests)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Test)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Test)

set(CMAKE_DEBUG_POSTFIX "")

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_BINARY_DIR}/McCore)

add_subdirectory(SimpleLibrary)
add_subdirectory(SimplePlugin)
add_subdirectory(CoreTest)
add_subdirectory(IocTest)
add_subdirectory(WidgetTest)
