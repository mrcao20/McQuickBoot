project(WidgetTest)

set(PROJECT_HEADERS
    TestWidget.h
)

set(PROJECT_SOURCES
    main.cpp
    TestWidget.cpp
)

set(PROJECT_RESOURCES
    resource.qrc
)

add_executable(${PROJECT_NAME}
    ${PROJECT_HEADERS}
    ${PROJECT_SOURCES}
    ${PROJECT_RESOURCES}
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets Qt6::Test
    McCore McIoc McWidget)
