INCLUDEPATH += $$PWD/include/

SOURCES += \
    $$PWD/src/Appender/McAbstractAppender.cpp \
    $$PWD/src/Appender/McConsoleAppender.cpp \
    $$PWD/src/Appender/McDailyRollingFileAppender.cpp \
    $$PWD/src/Appender/McFileAppender.cpp \
    $$PWD/src/Appender/McFileDeviceAppender.cpp \
    $$PWD/src/Appender/McRollingFileAppender.cpp \
    $$PWD/src/Appender/McSizeRollingFileAppender.cpp \
    $$PWD/src/Configurator/McINIConfigurator.cpp \
    $$PWD/src/Configurator/McSettingConfigurator.cpp \
    $$PWD/src/Configurator/McXMLConfigurator.cpp \
    $$PWD/src/Layout/McNormalLayout.cpp \
    $$PWD/src/Layout/McPatternLayout.cpp \
    $$PWD/src/Layout/McSimpleLayout.cpp \
    $$PWD/src/Layout/McTTCCLayout.cpp \
    $$PWD/src/Logger/McLogger.cpp \
    $$PWD/src/McLogManager.cpp \
    $$PWD/src/Repository/McLoggerRepository.cpp \
    $$PWD/src/Utils/McDeleteThreadWhenQuit.cpp

HEADERS += \
    $$PWD/include/McLog/Appender/IMcAppender.h \
    $$PWD/include/McLog/Appender/IMcConfigurableAppender.h \
    $$PWD/include/McLog/Appender/IMcWritableAppender.h \
    $$PWD/include/McLog/Appender/impl/McAbstractAppender.h \
    $$PWD/include/McLog/Appender/impl/McConsoleAppender.h \
    $$PWD/include/McLog/Appender/impl/McDailyRollingFileAppender.h \
    $$PWD/include/McLog/Appender/impl/McFileAppender.h \
    $$PWD/include/McLog/Appender/impl/McFileDeviceAppender.h \
    $$PWD/include/McLog/Appender/impl/McRollingFileAppender.h \
    $$PWD/include/McLog/Appender/impl/McSizeRollingFileAppender.h \
    $$PWD/include/McLog/Configurator/McINIConfigurator.h \
    $$PWD/include/McLog/Configurator/McSettingConfigurator.h \
    $$PWD/include/McLog/Configurator/McXMLConfigurator.h \
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
    $$PWD/include/McLog/McLogVersion.h \
    $$PWD/include/McLog/Repository/IMcLoggerRepository.h \
    $$PWD/include/McLog/Repository/impl/McLoggerRepository.h \
    $$PWD/include/McLog/Utils/McDeleteThreadWhenQuit.h
