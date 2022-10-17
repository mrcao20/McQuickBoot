include_directories(${CMAKE_CURRENT_LIST_DIR})

if(USE_LIBRARY)
    add_definitions(-DMC_LOG_LIBRARY)
endif()

list(APPEND PROJECT_HEADERS
    ${CMAKE_CURRENT_LIST_DIR}/McLogMacroGlobal.h
    ${CMAKE_CURRENT_LIST_DIR}/McLogGlobal.h
    ${CMAKE_CURRENT_LIST_DIR}/Utils/McMessagePattern.h
    ${CMAKE_CURRENT_LIST_DIR}/Utils/McFileUtils.h
    ${CMAKE_CURRENT_LIST_DIR}/Utils/McLogDeleter.h
    ${CMAKE_CURRENT_LIST_DIR}/Utils/McLogPackager.h
    ${CMAKE_CURRENT_LIST_DIR}/Appender/IMcAppender.h
    ${CMAKE_CURRENT_LIST_DIR}/Appender/IMcConfigurableAppender.h
    ${CMAKE_CURRENT_LIST_DIR}/Appender/IMcCodecableAppender.h
    ${CMAKE_CURRENT_LIST_DIR}/Appender/IMcRequestableNextFile.h
    ${CMAKE_CURRENT_LIST_DIR}/Appender/IMcWritableAppender.h
    ${CMAKE_CURRENT_LIST_DIR}/Appender/Impl/McAbstractAppender.h
    ${CMAKE_CURRENT_LIST_DIR}/Appender/Impl/McAbstractIODeviceAppender.h
    ${CMAKE_CURRENT_LIST_DIR}/Appender/Impl/McAbstractFormatAppender.h
    ${CMAKE_CURRENT_LIST_DIR}/Appender/Impl/McConsoleAppender.h
    ${CMAKE_CURRENT_LIST_DIR}/Appender/Impl/McFileAppender.h
    ${CMAKE_CURRENT_LIST_DIR}/Appender/Impl/McRollingFileAppender.h
    ${CMAKE_CURRENT_LIST_DIR}/Appender/Impl/McSizeRollingFileAppender.h
    ${CMAKE_CURRENT_LIST_DIR}/Appender/Impl/McByDayRollingFileAppender.h
    ${CMAKE_CURRENT_LIST_DIR}/Appender/Impl/McBySizeDayRollingFileAppender.h
    ${CMAKE_CURRENT_LIST_DIR}/Appender/Impl/McBySizeDailyRollingFileAppender.h
    ${CMAKE_CURRENT_LIST_DIR}/Appender/Impl/McDailyRollingFileAppender.h
    ${CMAKE_CURRENT_LIST_DIR}/Appender/Impl/McAppenderSeparator.h
    ${CMAKE_CURRENT_LIST_DIR}/Appender/Impl/McAbstractAppenderDecorator.h
    ${CMAKE_CURRENT_LIST_DIR}/Appender/Impl/McAppenderFrontDecorator.h
    ${CMAKE_CURRENT_LIST_DIR}/Appender/Impl/McAppenderPostDecorator.h
    ${CMAKE_CURRENT_LIST_DIR}/Layout/IMcLayout.h
    ${CMAKE_CURRENT_LIST_DIR}/Layout/Impl/McPatternLayout.h
    ${CMAKE_CURRENT_LIST_DIR}/Layout/Impl/McSimpleLayout.h
    ${CMAKE_CURRENT_LIST_DIR}/Layout/Impl/McNormalLayout.h
    ${CMAKE_CURRENT_LIST_DIR}/Layout/Impl/McTTCCLayout.h
    ${CMAKE_CURRENT_LIST_DIR}/Logger/IMcLogger.h
    ${CMAKE_CURRENT_LIST_DIR}/Logger/Impl/McLogger.h
    ${CMAKE_CURRENT_LIST_DIR}/Repository/IMcAdditionalTask.h
    ${CMAKE_CURRENT_LIST_DIR}/Repository/IMcLoggerRepository.h
    ${CMAKE_CURRENT_LIST_DIR}/Repository/Impl/McLoggerRepository.h
    ${CMAKE_CURRENT_LIST_DIR}/Configurator/McXMLConfigurator.h
    ${CMAKE_CURRENT_LIST_DIR}/McLogManager.h
)

list(APPEND PROJECT_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/McLogGlobal.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Utils/McMessagePattern.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Utils/McFileUtils.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Utils/McLogDeleter.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Utils/McLogPackager.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Appender/Impl/McAbstractAppender.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Appender/Impl/McAbstractIODeviceAppender.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Appender/Impl/McAbstractFormatAppender.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Appender/Impl/McConsoleAppender.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Appender/Impl/McFileAppender.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Appender/Impl/McRollingFileAppender.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Appender/Impl/McSizeRollingFileAppender.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Appender/Impl/McByDayRollingFileAppender.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Appender/Impl/McBySizeDayRollingFileAppender.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Appender/Impl/McBySizeDailyRollingFileAppender.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Appender/Impl/McDailyRollingFileAppender.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Appender/Impl/McAbstractAppenderDecorator.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Appender/Impl/McAppenderFrontDecorator.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Appender/Impl/McAppenderPostDecorator.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Layout/Impl/McPatternLayout.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Layout/Impl/McSimpleLayout.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Layout/Impl/McNormalLayout.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Layout/Impl/McTTCCLayout.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Logger/Impl/McLogger.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Repository/Impl/McLoggerRepository.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Configurator/McXMLConfigurator.cpp
    ${CMAKE_CURRENT_LIST_DIR}/McLogManager.cpp
)

if(WIN32)
    list(APPEND PROJECT_HEADERS
        ${CMAKE_CURRENT_LIST_DIR}/Device/McVSDebugDevice.h
        ${CMAKE_CURRENT_LIST_DIR}/Appender/Impl/McVSDebugAppender.h
    )

    list(APPEND PROJECT_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/Device/McVSDebugDevice.cpp
        ${CMAKE_CURRENT_LIST_DIR}/Appender/Impl/McVSDebugAppender.cpp
    )
endif()
