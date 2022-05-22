project(McWidget)

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_BINARY_DIR}/McCore)

set(PROJECT_HEADERS
    McWidgetMacroGlobal.h
    McWidgetGlobal.h
    BeanBuilder/Impl/McWidgetBeanBuilder.h
    BeanBuilder/Impl/McCustomWidgetBeanBuilder.h
    BeanBuilderReader/Impl/McXmlWidgetBeanBuilderReader.h
    ApplicationContext/Impl/McXmlWidgetApplicationContext.h
    ApplicationContext/Impl/McLocalPathWidgetApplicationContext.h
)

set(PROJECT_SOURCES
    McWidgetGlobal.cpp
    BeanBuilder/Impl/McWidgetBeanBuilder.cpp
    BeanBuilderReader/Impl/McXmlWidgetBeanBuilderReader.cpp
    ApplicationContext/Impl/McXmlWidgetApplicationContext.cpp
    ApplicationContext/Impl/McLocalPathWidgetApplicationContext.cpp
)

add_library(${PROJECT_NAME} ${MC_BUILD_TYPE}
    ${PROJECT_HEADERS}
    ${PROJECT_SOURCES}
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core Qt6::Widgets
    McCore McIoc)

target_compile_definitions(${PROJECT_NAME} PRIVATE MC_WIDGET_LIBRARY)

include(${CMAKE_SOURCE_DIR}/cmake/Install.cmake)
