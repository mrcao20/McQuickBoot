project(CoreExample)

set(PROJECT_HEADERS
    MainWindow.h
    MetaTypeRegister.h
)

set(PROJECT_SOURCES
    main.cpp
    MainWindow.cpp
    MetaTypeRegister.cpp
)

set(PROJECT_FORMS
    MainWindow.ui
)

add_executable(${PROJECT_NAME}
    ${PROJECT_HEADERS}
    ${PROJECT_SOURCES}
    ${PROJECT_FORMS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets McCore)
