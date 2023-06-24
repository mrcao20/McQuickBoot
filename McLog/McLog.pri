INCLUDEPATH += $$PWD

contains(DEFINES, MC_USE_LIBRARY) {
    DEFINES += MC_LOG_LIBRARY
}

HEADERS += \
    $$PWD/McLogMacroGlobal.h \
    $$PWD/McLogGlobal.h \
    $$PWD/Utils/McMessagePattern.h \
    $$PWD/Utils/McFileUtils.h \
    $$PWD/Utils/McLogDeleter.h \
    $$PWD/Appender/IMcAppender.h \
    $$PWD/Appender/IMcConfigurableAppender.h \
    $$PWD/Appender/IMcCodecableAppender.h \
    $$PWD/Appender/IMcRequestableNextFile.h \
    $$PWD/Appender/IMcWritableAppender.h \
    $$PWD/Appender/Impl/McAbstractAppender.h \
    $$PWD/Appender/Impl/McAbstractIODeviceAppender.h \
    $$PWD/Appender/Impl/McAbstractFormatAppender.h \
    $$PWD/Appender/Impl/McConsoleAppender.h \
    $$PWD/Appender/Impl/McFileAppender.h \
    $$PWD/Appender/Impl/McRollingFileAppender.h \
    $$PWD/Appender/Impl/McSizeRollingFileAppender.h \
    $$PWD/Appender/Impl/McByDayRollingFileAppender.h \
    $$PWD/Appender/Impl/McBySizeDayRollingFileAppender.h \
    $$PWD/Appender/Impl/McBySizeDailyRollingFileAppender.h \
    $$PWD/Appender/Impl/McDailyRollingFileAppender.h \
    $$PWD/Appender/Impl/McAppenderSeparator.h \
    $$PWD/Appender/Impl/McAbstractAppenderDecorator.h \
    $$PWD/Appender/Impl/McAppenderFrontDecorator.h \
    $$PWD/Appender/Impl/McAppenderPostDecorator.h \
    $$PWD/Layout/IMcLayout.h \
    $$PWD/Layout/Impl/McPatternLayout.h \
    $$PWD/Layout/Impl/McSimpleLayout.h \
    $$PWD/Layout/Impl/McNormalLayout.h \
    $$PWD/Layout/Impl/McTTCCLayout.h \
    $$PWD/Logger/IMcLogger.h \
    $$PWD/Logger/Impl/McLogger.h \
    $$PWD/Repository/IMcAdditionalTask.h \
    $$PWD/Repository/IMcLoggerRepository.h \
    $$PWD/Repository/Impl/McLoggerRepository.h \
    $$PWD/Configurator/McXMLConfigurator.h \
    $$PWD/McLogManager.h

SOURCES += \
    $$PWD/McLogGlobal.cpp \
    $$PWD/Utils/McMessagePattern.cpp \
    $$PWD/Utils/McFileUtils.cpp \
    $$PWD/Utils/McLogDeleter.cpp \
    $$PWD/Appender/Impl/McAbstractAppender.cpp \
    $$PWD/Appender/Impl/McAbstractIODeviceAppender.cpp \
    $$PWD/Appender/Impl/McAbstractFormatAppender.cpp \
    $$PWD/Appender/Impl/McConsoleAppender.cpp \
    $$PWD/Appender/Impl/McFileAppender.cpp \
    $$PWD/Appender/Impl/McRollingFileAppender.cpp \
    $$PWD/Appender/Impl/McSizeRollingFileAppender.cpp \
    $$PWD/Appender/Impl/McByDayRollingFileAppender.cpp \
    $$PWD/Appender/Impl/McBySizeDayRollingFileAppender.cpp \
    $$PWD/Appender/Impl/McBySizeDailyRollingFileAppender.cpp \
    $$PWD/Appender/Impl/McDailyRollingFileAppender.cpp \
    $$PWD/Appender/Impl/McAbstractAppenderDecorator.cpp \
    $$PWD/Appender/Impl/McAppenderFrontDecorator.cpp \
    $$PWD/Appender/Impl/McAppenderPostDecorator.cpp \
    $$PWD/Layout/Impl/McPatternLayout.cpp \
    $$PWD/Layout/Impl/McSimpleLayout.cpp \
    $$PWD/Layout/Impl/McNormalLayout.cpp \
    $$PWD/Layout/Impl/McTTCCLayout.cpp \
    $$PWD/Logger/Impl/McLogger.cpp \
    $$PWD/Repository/Impl/McLoggerRepository.cpp \
    $$PWD/Configurator/McXMLConfigurator.cpp \
    $$PWD/McLogManager.cpp

!contains(DEFINES, MC_DISABLE_ZLIB) {
    HEADERS += $$PWD/Utils/McLogPackager.h
    SOURCES += $$PWD/Utils/McLogPackager.cpp
}

win32 {
    HEADERS += \
        $$PWD/Device/McVSDebugDevice.h \
        $$PWD/Appender/Impl/McVSDebugAppender.h

    SOURCES += \
        $$PWD/Device/McVSDebugDevice.cpp \
        $$PWD/Appender/Impl/McVSDebugAppender.cpp
}
