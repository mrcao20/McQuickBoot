# 注意LANGUAGES必须同时包含C和CXX，因为本库由C++编写，但又链接了其他第三方C库

include_directories(${CMAKE_CURRENT_LIST_DIR})

option(USE_LIBRARY "如果将此库放在程序的某一个库中，其他模块链接到该库使用时打开此配置" ON)
option(EXPORT_DISABLE "如果将此库直接链接到程序，不放在任何库中时打开此配置" OFF)

if(EXPORT_DISABLE)
    add_definitions(-DMC_EXPORT_DISABLE)
endif()

include(${CMAKE_CURRENT_LIST_DIR}/McCore/McCore.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/McIoc/McIoc.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/McLog/McLog.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/McQuickBoot/McQuickBoot.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/McWidget/McWidget.cmake)
