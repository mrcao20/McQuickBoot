project(LogExample)

set(PROJECT_HEADERS
    MainWindow.h
)

set(PROJECT_SOURCES
    main.cpp
    MainWindow.cpp
)

set(PROJECT_FORMS
    MainWindow.ui
)

set(PROJECT_RESOURCES
    resource.qrc
)

add_executable(${PROJECT_NAME}
    ${PROJECT_HEADERS}
    ${PROJECT_SOURCES}
    ${PROJECT_FORMS}
    ${PROJECT_RESOURCES}
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Concurrent
    McCore McLog)
