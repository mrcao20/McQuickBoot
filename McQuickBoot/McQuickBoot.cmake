include_directories(${CMAKE_CURRENT_LIST_DIR})

if(USE_LIBRARY)
    add_definitions(-DMC_QUICKBOOT_LIBRARY)
endif()

list(APPEND PROJECT_HEADERS
    ${CMAKE_CURRENT_LIST_DIR}/McQuickBootMacroGlobal.h
    ${CMAKE_CURRENT_LIST_DIR}/McQuickBootGlobal.h
)

list(APPEND PROJECT_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/McQuickBootGlobal.cpp
)
