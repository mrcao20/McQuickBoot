project(McIoc)

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_BINARY_DIR}/McCore)

set(PROJECT_HEADERS
    McIocMacroGlobal.h
    McIocConstantGlobal.h
    McIocGlobal.h
    McGlobal.h
    BeanFactory/IMcBeanFactory.h
)

set(PROJECT_SOURCES
    McIocGlobal.cpp
)

add_library(${PROJECT_NAME} ${MC_BUILD_TYPE}
    ${PROJECT_HEADERS}
    ${PROJECT_SOURCES}
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core)

target_compile_definitions(${PROJECT_NAME} PRIVATE MC_IOC_LIBRARY)

include(${CMAKE_SOURCE_DIR}/cmake/Install.cmake)
