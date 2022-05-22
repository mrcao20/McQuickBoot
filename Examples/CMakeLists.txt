project(Examples)

set(CMAKE_DEBUG_POSTFIX "")

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_BINARY_DIR}/McCore)

add_subdirectory(CoreExample)
add_subdirectory(IocExample)
add_subdirectory(LogExample)
add_subdirectory(WidgetExample)
add_subdirectory(LibraryExample)
add_subdirectory(PluginExample)
add_subdirectory(PluginExample2)
