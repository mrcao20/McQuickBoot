include_directories(${CMAKE_CURRENT_LIST_DIR})

if(USE_LIBRARY)
    add_definitions(-DMC_QUICKBOOT_LIBRARY)
endif()

list(APPEND PROJECT_HEADERS
    ${CMAKE_CURRENT_LIST_DIR}/McQuickBootMacroGlobal.h
    ${CMAKE_CURRENT_LIST_DIR}/McQuickBootGlobal.h
    ${CMAKE_CURRENT_LIST_DIR}/Invoker/McInvoker.h
    ${CMAKE_CURRENT_LIST_DIR}/Invoker/McAbstractPromise.h
    ${CMAKE_CURRENT_LIST_DIR}/Invoker/McCppPromise.h
    ${CMAKE_CURRENT_LIST_DIR}/Invoker/McResult.h
)

list(APPEND PROJECT_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/McQuickBootGlobal.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Invoker/McInvoker.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Invoker/McAbstractPromise.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Invoker/McCppPromise.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Invoker/McResult.cpp
)
