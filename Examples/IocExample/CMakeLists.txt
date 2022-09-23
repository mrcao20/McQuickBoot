project(IocExample)

set(PROJECT_HEADERS
    MainWindow.h
    Domain/SimplePod.h
    Domain/SimpleGadget.h
    Domain/ExampleObject.h
    PluginChecker.h
)

set(PROJECT_SOURCES
    main.cpp
    MainWindow.cpp
    Domain/SimplePod.cpp
    Domain/SimpleGadget.cpp
    Domain/ExampleObject.cpp
    PluginChecker.cpp
)

set(PROJECT_FORMS
    MainWindow.ui
)

add_executable(${PROJECT_NAME}
    ${PROJECT_HEADERS}
    ${PROJECT_SOURCES}
    ${PROJECT_FORMS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets McCore McIoc)
