project(CoreTest)

set(PROJECT_HEADERS
    TestCore.h
    MetaTypeTest.h
)

set(PROJECT_SOURCES
    main.cpp
    TestCore.cpp
    MetaTypeTest.cpp
)

add_executable(${PROJECT_NAME}
    ${PROJECT_HEADERS}
    ${PROJECT_SOURCES}
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Test Qt${QT_VERSION_MAJOR}::Concurrent McCore)
