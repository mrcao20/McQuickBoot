project(PluginExample2)

set(PROJECT_HEADERS
)

set(PROJECT_SOURCES
)

add_library(${PROJECT_NAME} SHARED
    ${PROJECT_HEADERS}
    ${PROJECT_SOURCES}
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core McCore McIoc)

if (MINGW)
    set_target_properties (${PROJECT_NAME} PROPERTIES PREFIX "")
endif ()
