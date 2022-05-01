project(LibraryExample)

set(PROJECT_HEADERS
    SimpleObject.h
)

set(PROJECT_SOURCES
    LibraryExample.cpp
    SimpleObject.cpp
)

add_library(${PROJECT_NAME} SHARED
    ${PROJECT_HEADERS}
    ${PROJECT_SOURCES}
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core McCore McIoc)

if (MINGW)
    set_target_properties (${PROJECT_NAME} PROPERTIES PREFIX "")
endif ()
