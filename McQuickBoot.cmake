# 注意不能填写LANGUAGES，填C无法编译C++的符号，填CXX则无法编译本库所链接的其他第三方C库

include_directories(${CMAKE_CURRENT_LIST_DIR})

add_definitions(-DMC_EXPORT_DISABLE)

include(${CMAKE_CURRENT_LIST_DIR}/McCore/McCore.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/McIoc/McIoc.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/McLog/McLog.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/McQuickBoot/McQuickBoot.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/McWidget/McWidget.cmake)
