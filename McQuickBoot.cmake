# 注意LANGUAGES必须同时包含C和CXX，因为本库由C++编写，但又链接了其他第三方C库

include_directories(${CMAKE_CURRENT_LIST_DIR})

add_definitions(-DMC_EXPORT_DISABLE)

include(${CMAKE_CURRENT_LIST_DIR}/McCore/McCore.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/McIoc/McIoc.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/McLog/McLog.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/McQuickBoot/McQuickBoot.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/McWidget/McWidget.cmake)
