include_directories(${CMAKE_CURRENT_LIST_DIR})

if(NOT DISABLE_ZLIB)
    include(${CMAKE_CURRENT_LIST_DIR}/zlib/zlib.cmake)
endif()
include(${CMAKE_CURRENT_LIST_DIR}/yaml-cpp/yaml-cpp.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/google_breakpad/google_breakpad.cmake)
