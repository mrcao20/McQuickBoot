project(SimplePlugin)

include_directories(../IocTest)

set(PROJECT_HEADERS
    SimplePlugin.h
)

set(PROJECT_SOURCES
    SimplePlugin.cpp
)

add_library(${PROJECT_NAME} SHARED
    ${PROJECT_HEADERS}
    ${PROJECT_SOURCES}
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Core McCore)

if (MINGW)
    set_target_properties (${PROJECT_NAME} PROPERTIES PREFIX "")
endif ()
