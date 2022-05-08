project(McLog)

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_BINARY_DIR}/McCore)

set(PROJECT_HEADERS
    McLogMacroGlobal.h
    McLogGlobal.h
    Utils/McMessagePattern.h
    Utils/McFileUtils.h
    Appender/IMcAppender.h
    Appender/IMcConfigurableAppender.h
    Appender/IMcCodecableAppender.h
    Appender/IMcRequestableNextFile.h
    Appender/IMcWritableAppender.h
    Appender/Impl/McAbstractAppender.h
    Appender/Impl/McAbstractIODeviceAppender.h
    Appender/Impl/McAbstractFormatAppender.h
    Appender/Impl/McConsoleAppender.h
    Appender/Impl/McFileAppender.h
    Appender/Impl/McRollingFileAppender.h
    Appender/Impl/McSizeRollingFileAppender.h
    Appender/Impl/McByDayRollingFileAppender.h
    Appender/Impl/McBySizeDayRollingFileAppender.h
    Appender/Impl/McBySizeDailyRollingFileAppender.h
    Appender/Impl/McDailyRollingFileAppender.h
    Layout/IMcLayout.h
    Layout/Impl/McPatternLayout.h
    Layout/Impl/McSimpleLayout.h
    Layout/Impl/McNormalLayout.h
    Layout/Impl/McTTCCLayout.h
)

set(PROJECT_SOURCES
    McLogGlobal.cpp
    Utils/McMessagePattern.cpp
    Utils/McFileUtils.cpp
    Appender/Impl/McAbstractAppender.cpp
    Appender/Impl/McAbstractIODeviceAppender.cpp
    Appender/Impl/McAbstractFormatAppender.cpp
    Appender/Impl/McConsoleAppender.cpp
    Appender/Impl/McFileAppender.cpp
    Appender/Impl/McRollingFileAppender.cpp
    Appender/Impl/McSizeRollingFileAppender.cpp
    Appender/Impl/McByDayRollingFileAppender.cpp
    Appender/Impl/McBySizeDayRollingFileAppender.cpp
    Appender/Impl/McBySizeDailyRollingFileAppender.cpp
    Appender/Impl/McDailyRollingFileAppender.cpp
    Layout/Impl/McPatternLayout.cpp
    Layout/Impl/McSimpleLayout.cpp
    Layout/Impl/McNormalLayout.cpp
    Layout/Impl/McTTCCLayout.cpp
    Appender/Impl/McVSDebugAppender.h
)

if(WIN32)
    list(APPEND PROJECT_HEADERS Device/McVSDebugDevice.h)
    list(APPEND PROJECT_HEADERS Appender/Impl/McVSDebugAppender.h)
    list(APPEND PROJECT_SOURCES Device/McVSDebugDevice.cpp)
    list(APPEND PROJECT_SOURCES Appender/Impl/McVSDebugAppender.cpp)
endif()

add_library(${PROJECT_NAME} ${MC_BUILD_TYPE}
    ${PROJECT_HEADERS}
    ${PROJECT_SOURCES}
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core
    McCore McIoc)

target_compile_definitions(${PROJECT_NAME} PRIVATE MC_LOG_LIBRARY)

include(${CMAKE_SOURCE_DIR}/cmake/Install.cmake)
