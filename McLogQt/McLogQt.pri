QT *= concurrent

!contains(DEFINES, MC_DISABLE_QUAZIP) {
    HEADERS += $$PWD/include/McLog/Utils/Packager/McLogPackager.h
    SOURCES += $$PWD/src/Utils/Packager/McLogPackager.cpp
}

INCLUDEPATH += $$PWD/include/

SOURCES += \
    $$PWD/src/Appender/Decorator/McAbstractAppenderDecorator.cpp \
    $$PWD/src/Appender/Decorator/McAppenderFrontDecorator.cpp \
    $$PWD/src/Appender/Decorator/McAppenderPostDecorator.cpp \
    $$PWD/src/Appender/McAbstractAppender.cpp \
    $$PWD/src/Appender/McByDayRollingFileAppender.cpp \
    $$PWD/src/Appender/McBySizeDailyRollingFileAppender.cpp \
    $$PWD/src/Appender/McBySizeDayRollingFileAppender.cpp \
    $$PWD/src/Appender/McConsoleAppender.cpp \
    $$PWD/src/Appender/McDailyRollingFileAppender.cpp \
    $$PWD/src/Appender/McFileAppender.cpp \
    $$PWD/src/Appender/McFileDeviceAppender.cpp \
    $$PWD/src/Appender/McRollingFileAppender.cpp \
    $$PWD/src/Appender/McSizeRollingFileAppender.cpp \
    $$PWD/src/Configurator/McDefaultConfigurator.cpp \
    $$PWD/src/Configurator/McINIConfigurator.cpp \
    $$PWD/src/Configurator/McSettingConfigurator.cpp \
    $$PWD/src/Configurator/McXMLConfigurator.cpp \
    $$PWD/src/Device/McVSDebugDevice.cpp \
    $$PWD/src/Layout/McNormalLayout.cpp \
    $$PWD/src/Layout/McPatternLayout.cpp \
    $$PWD/src/Layout/McSimpleLayout.cpp \
    $$PWD/src/Layout/McTTCCLayout.cpp \
    $$PWD/src/Logger/McLogger.cpp \
    $$PWD/src/Appender/McAbstractIODeviceAppender.cpp \
    $$PWD/src/McLogManager.cpp \
    $$PWD/src/Appender/McVSDebugAppender.cpp \
    $$PWD/src/Repository/McLoggerRepository.cpp \
    $$PWD/src/Appender/McAbstractFormatAppender.cpp \
    $$PWD/src/Utils/Deleter/McLogDeleter.cpp \
    $$PWD/src/Utils/McFileUtils.cpp \
    $$PWD/src/Utils/McMessagePattern.cpp

HEADERS += \
    $$PWD/include/McLog/Appender/Decorator/McAbstractAppenderDecorator.h \
    $$PWD/include/McLog/Appender/Decorator/McAppenderFrontDecorator.h \
    $$PWD/include/McLog/Appender/Decorator/McAppenderPostDecorator.h \
    $$PWD/include/McLog/Appender/Decorator/McAppenderSeparator.h \
    $$PWD/include/McLog/Appender/IMcAppender.h \
    $$PWD/include/McLog/Appender/IMcCodecableAppender.h \
    $$PWD/include/McLog/Appender/IMcConfigurableAppender.h \
    $$PWD/include/McLog/Appender/IMcRequestableNextFile.h \
    $$PWD/include/McLog/Appender/IMcWritableAppender.h \
    $$PWD/include/McLog/Appender/impl/McAbstractAppender.h \
    $$PWD/include/McLog/Appender/impl/McAbstractIODeviceAppender.h \
    $$PWD/include/McLog/Appender/impl/McByDayRollingFileAppender.h \
    $$PWD/include/McLog/Appender/impl/McBySizeDailyRollingFileAppender.h \
    $$PWD/include/McLog/Appender/impl/McBySizeDayRollingFileAppender.h \
    $$PWD/include/McLog/Appender/impl/McConsoleAppender.h \
    $$PWD/include/McLog/Appender/impl/McDailyRollingFileAppender.h \
    $$PWD/include/McLog/Appender/impl/McFileAppender.h \
    $$PWD/include/McLog/Appender/impl/McFileDeviceAppender.h \
    $$PWD/include/McLog/Appender/impl/McRollingFileAppender.h \
    $$PWD/include/McLog/Appender/impl/McSizeRollingFileAppender.h \
    $$PWD/include/McLog/Appender/impl/McVSDebugAppender.h \
    $$PWD/include/McLog/Configurator/McDefaultConfigurator.h \
    $$PWD/include/McLog/Configurator/McINIConfigurator.h \
    $$PWD/include/McLog/Configurator/McSettingConfigurator.h \
    $$PWD/include/McLog/Configurator/McXMLConfigurator.h \
    $$PWD/include/McLog/Device/McVSDebugDevice.h \
    $$PWD/include/McLog/Layout/IMcLayout.h \
    $$PWD/include/McLog/Layout/impl/McNormalLayout.h \
    $$PWD/include/McLog/Layout/impl/McPatternLayout.h \
    $$PWD/include/McLog/Layout/impl/McSimpleLayout.h \
    $$PWD/include/McLog/Layout/impl/McTTCCLayout.h \
    $$PWD/include/McLog/Logger/IMcLogger.h \
    $$PWD/include/McLog/Logger/impl/McLogger.h \
    $$PWD/include/McLog/McLogGlobal.h \
    $$PWD/include/McLog/McLogMacroGlobal.h \
    $$PWD/include/McLog/McLogManager.h \
    $$PWD/include/McLog/Repository/IMcAdditionalTask.h \
    $$PWD/include/McLog/Repository/IMcLoggerRepository.h \
    $$PWD/include/McLog/Repository/impl/McLoggerRepository.h \
    $$PWD/include/McLog/Appender/impl/McAbstractFormatAppender.h \
    $$PWD/include/McLog/Utils/Deleter/McLogDeleter.h \
    $$PWD/include/McLog/Utils/McFileUtils.h \
    $$PWD/include/McLog/Utils/McMessagePattern.h

RESOURCES +=
