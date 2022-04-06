project(CoreTest)

set(PROJECT_HEADERS
)

set(PROJECT_SOURCES
)

add_executable(${PROJECT_NAME}
    ${PROJECT_HEADERS}
    ${PROJECT_SOURCES}
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Test McCore)
