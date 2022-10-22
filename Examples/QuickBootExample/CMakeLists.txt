project(QuickBootExample)

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

add_executable(${PROJECT_NAME}
    ${PROJECT_HEADERS}
    ${PROJECT_SOURCES}
    ${PROJECT_FORMS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets McCore McQuickBoot)
