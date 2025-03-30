include_directories(${CMAKE_CURRENT_LIST_DIR})

if(USE_LIBRARY)
    add_definitions(-DMC_WIDGET_LIBRARY)
endif()

list(APPEND PROJECT_HEADERS
    ${CMAKE_CURRENT_LIST_DIR}/McWidgetMacroGlobal.h
    ${CMAKE_CURRENT_LIST_DIR}/McWidgetGlobal.h
    ${CMAKE_CURRENT_LIST_DIR}/BeanBuilder/Impl/McWidgetBeanBuilder.h
    ${CMAKE_CURRENT_LIST_DIR}/BeanBuilder/Impl/McCustomWidgetBeanBuilder.h
    ${CMAKE_CURRENT_LIST_DIR}/BeanBuilderReader/Impl/McXmlWidgetBeanBuilderReader.h
    ${CMAKE_CURRENT_LIST_DIR}/ApplicationContext/Impl/McXmlWidgetApplicationContext.h
    ${CMAKE_CURRENT_LIST_DIR}/ApplicationContext/Impl/McLocalPathWidgetApplicationContext.h
)

list(APPEND PROJECT_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/McWidgetGlobal.cpp
    ${CMAKE_CURRENT_LIST_DIR}/BeanBuilder/Impl/McWidgetBeanBuilder.cpp
    ${CMAKE_CURRENT_LIST_DIR}/BeanBuilderReader/Impl/McXmlWidgetBeanBuilderReader.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ApplicationContext/Impl/McXmlWidgetApplicationContext.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ApplicationContext/Impl/McLocalPathWidgetApplicationContext.cpp
)
