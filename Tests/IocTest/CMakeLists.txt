project(IocTest)

set(PROJECT_HEADERS
    BeanFactoryTest.h
    IObjectTest.h
)

set(PROJECT_SOURCES
    main.cpp
    BeanFactoryTest.cpp
)

add_executable(${PROJECT_NAME}
    ${PROJECT_HEADERS}
    ${PROJECT_SOURCES}
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Test
    McCore McIoc)
