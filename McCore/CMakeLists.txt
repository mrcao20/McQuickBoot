project(McCore)

configure_file(McConfig.h.in McConfig.h)

include_directories(${Qt6Core_PRIVATE_INCLUDE_DIRS})

set(PROJECT_HEADERS
    McMacroGlobal.h
    McGlobal.h
    Event/McEventDispatcher.h
    Event/McEventRouter.h
    Utils/McQVariantConverter.h
    Utils/IMcNonCopyable.h
)

set(PROJECT_SOURCES
    McGlobal.cpp
    Event/McEventDispatcher.cpp
    Event/McEventRouter.cpp
)

add_library(${PROJECT_NAME} SHARED
    ${PROJECT_HEADERS}
    ${PROJECT_SOURCES}
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core)

target_compile_definitions(${PROJECT_NAME} PRIVATE MC_CORE_LIBRARY)

include(${CMAKE_SOURCE_DIR}/cmake/Install.cmake)
