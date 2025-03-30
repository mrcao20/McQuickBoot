project(PluginExample2)

set(PROJECT_HEADERS
    SimplePlugin2.h
)

set(PROJECT_SOURCES
    SimplePlugin2.cpp
)

add_library(${PROJECT_NAME} SHARED
    ${PROJECT_HEADERS}
    ${PROJECT_SOURCES}
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Core McCore McIoc)

if (MINGW)
    set_target_properties (${PROJECT_NAME} PROPERTIES PREFIX "")
endif ()
