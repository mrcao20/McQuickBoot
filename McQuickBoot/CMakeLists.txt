project(McQuickBoot)

set(PROJECT_HEADERS
    McQuickBootMacroGlobal.h
    McQuickBootConstantGlobal.h
    McQuickBootGlobal.h
    Invoker/McInvoker.h
    Invoker/McAbstractPromise.h
    Invoker/McCppPromise.h
    Invoker/McResult.h
    Invoker/McAbstractInvoker.h
    Invoker/McCppInvoker.h
    ComponentContainer/McControllerContainer.h
    ComponentContainer/McControllerEntity.h
    QuickBoot/McAbstractQuickBoot.h
    QuickBoot/McCppQuickBoot.h
    QuickBoot/McQuickBoot.h
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
    QuickBoot/McCppQuickBoot.cpp
)

if(ENABLE_QML)
    list(APPEND PROJECT_HEADERS
        QuickBoot/McQmlQuickBoot.h
        Invoker/McQmlInvoker.h
        Invoker/McQmlPromise.h
        Utils/McQmlSyncCallback.h
    )

    list(APPEND PROJECT_SOURCES
        QuickBoot/McQmlQuickBoot.cpp
        QuickBoot/McQuickBoot.cpp
        Invoker/McQmlInvoker.cpp
        Invoker/McQmlPromise.cpp
        Utils/McQmlSyncCallback.cpp
    )
endif()

qt_add_resources(PROJECT_SOURCES Resources/Resources.qrc)

add_library(${PROJECT_NAME} ${MC_BUILD_TYPE}
    ${PROJECT_HEADERS}
    ${PROJECT_SOURCES}
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Core
    McCore McIoc)

if(ENABLE_QML)
    target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Gui Qt${QT_VERSION_MAJOR}::Qml)
endif()

target_compile_definitions(${PROJECT_NAME} PRIVATE MC_QUICKBOOT_LIBRARY)

include(${CMAKE_SOURCE_DIR}/cmake/Install.cmake)
