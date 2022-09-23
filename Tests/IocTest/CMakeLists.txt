project(IocTest)

set(PROJECT_HEADERS
    BeanFactoryTest.h
    BeanReaderTest.h
    XmlApplicationContextTest.h
    AnnotationApplicationContextTest.h
    XmlBuilder.h
    IObjectTest.h
)

set(PROJECT_SOURCES
    main.cpp
    BeanFactoryTest.cpp
    BeanReaderTest.cpp
    XmlApplicationContextTest.cpp
    AnnotationApplicationContextTest.cpp
    XmlBuilder.cpp
)

set(PROJECT_RESOURCES
    resource.qrc
)

add_executable(${PROJECT_NAME}
    ${PROJECT_HEADERS}
    ${PROJECT_SOURCES}
    ${PROJECT_RESOURCES}
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Test
    McCore McIoc)
