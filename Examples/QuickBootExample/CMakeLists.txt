project(QuickBootExample)

set(PROJECT_HEADERS
    MainWindow.h
    Controller/TestController.h
    Service/TestService.h
    Domain/RequestVo.h
)

set(PROJECT_SOURCES
    main.cpp
    MainWindow.cpp
    Controller/TestController.cpp
    Service/TestService.cpp
    Domain/DomainGlobal.cpp
)

set(PROJECT_FORMS
    MainWindow.ui
)

if(ENABLE_QML)
    qt_add_resources(PROJECT_SOURCES Qml/Qml.qrc)
endif()

add_executable(${PROJECT_NAME}
    ${PROJECT_HEADERS}
    ${PROJECT_SOURCES}
    ${PROJECT_FORMS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets McCore McQuickBoot)

if(ENABLE_QML)
    target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Gui Qt${QT_VERSION_MAJOR}::Qml)
endif()
