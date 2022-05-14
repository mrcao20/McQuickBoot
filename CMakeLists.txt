cmake_minimum_required(VERSION 3.21)

set(MC_QUICKBOOT_VERSION_MAJOR 2)
set(MC_QUICKBOOT_VERSION_MINOR 1)
set(MC_QUICKBOOT_VERSION_PATCH 0)
set(MC_QUICKBOOT_VERSION ${MC_QUICKBOOT_VERSION_MAJOR}.${MC_QUICKBOOT_VERSION_MINOR}.${MC_QUICKBOOT_VERSION_PATCH})

project(McQuickBoot
    VERSION ${MC_QUICKBOOT_VERSION}
    DESCRIPTION "McQuickBoot Libraries"
    HOMEPAGE_URL "https://github.com/mrcao20/McQuickBoot"
    LANGUAGES CXX)

if(MSVC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /utf-8")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /utf-8")
endif()

# 不需要小写的QT宏
add_definitions(-DQT_NO_KEYWORDS)

if(!CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

if(NOT (CMAKE_BUILD_TYPE STREQUAL "Debug"))
    add_definitions(-DQT_MESSAGELOGCONTEXT)
endif()

option(BUILD_EXAMPLES "Build examples" ON)
option(BUILD_TESTS "Build tests. depend qtest" ON)
option(BUILD_STATIC "Build static" OFF)
option(INSTALL_TO_QT "Install output to qt location" OFF)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 COMPONENTS Core Widgets Concurrent REQUIRED)

set(CMAKE_DEBUG_POSTFIX "d")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/output CACHE PATH "Default install prefix" FORCE)
endif()

set(MC_QUICKBOOT_INSTALL_HEADERS ${CMAKE_INSTALL_PREFIX}/include)
set(MC_QUICKBOOT_INSTALL_LIBS ${CMAKE_INSTALL_PREFIX}/lib)
set(MC_QUICKBOOT_INSTALL_BINS ${CMAKE_INSTALL_PREFIX}/bin)

if(BUILD_STATIC)
    set(MC_BUILD_TYPE STATIC)
    add_definitions(-DMC_BUILD_STATIC)
else()
    set(MC_BUILD_TYPE SHARED)
endif()

if(INSTALL_TO_QT)
    get_target_property(qmake Qt6::qmake LOCATION)
    execute_process(
        COMMAND ${qmake} -query QT_INSTALL_HEADERS
        OUTPUT_VARIABLE QT_INSTALL_HEADERS
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    execute_process(
        COMMAND ${qmake} -query QT_INSTALL_LIBS
        OUTPUT_VARIABLE QT_INSTALL_LIBS
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    execute_process(
        COMMAND ${qmake} -query QT_INSTALL_BINS
        OUTPUT_VARIABLE QT_INSTALL_BINS
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    execute_process(
        COMMAND ${qmake} -query QT_INSTALL_PREFIX
        OUTPUT_VARIABLE QT_INSTALL_PREFIX
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    list(APPEND MC_QUICKBOOT_INSTALL_HEADERS ${QT_INSTALL_HEADERS})
    list(APPEND MC_QUICKBOOT_INSTALL_LIBS ${QT_INSTALL_LIBS})
    list(APPEND MC_QUICKBOOT_INSTALL_BINS ${QT_INSTALL_BINS})
endif()

add_subdirectory(McCore)
add_subdirectory(McIoc)
add_subdirectory(McLog)
add_subdirectory(McWidget)
if(BUILD_EXAMPLES)
    add_subdirectory(Examples)
endif()
if(BUILD_TESTS)
    add_subdirectory(Tests)
endif()
