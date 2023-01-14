project(McQuickBoot)

set(PROJECT_HEADERS
    McQuickBootMacroGlobal.h
    McQuickBootGlobal.h
    Invoker/McInvoker.h
    Invoker/McAbstractPromise.h
    Invoker/McCppPromise.h
    Invoker/McResult.h
    Invoker/McAbstractInvoker.h
    Invoker/McCppInvoker.h
    ComponentContainer/McControllerContainer.h
    QuickBoot/McAbstractQuickBoot.h
)

set(PROJECT_SOURCES
    McQuickBootGlobal.cpp
    Invoker/McInvoker.cpp
    Invoker/McAbstractPromise.cpp
    Invoker/McCppPromise.cpp
    Invoker/McResult.cpp
    Invoker/McAbstractInvoker.cpp
    Invoker/McCppInvoker.cpp
    ComponentContainer/McControllerContainer.cpp
    QuickBoot/McAbstractQuickBoot.cpp
)

add_library(${PROJECT_NAME} ${MC_BUILD_TYPE}
    ${PROJECT_HEADERS}
    ${PROJECT_SOURCES}
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Core
    McCore McIoc)

target_compile_definitions(${PROJECT_NAME} PRIVATE MC_QUICKBOOT_LIBRARY)

include(${CMAKE_SOURCE_DIR}/cmake/Install.cmake)
