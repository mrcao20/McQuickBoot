INCLUDEPATH += $$PWD

contains(DEFINES, MC_USE_LIBRARY) {
    DEFINES += MC_WIDGET_LIBRARY
}

HEADERS += \
    $$PWD/McWidgetMacroGlobal.h \
    $$PWD/McWidgetGlobal.h \
    $$PWD/BeanBuilder/Impl/McWidgetBeanBuilder.h \
    $$PWD/BeanBuilder/Impl/McCustomWidgetBeanBuilder.h \
    $$PWD/BeanBuilderReader/Impl/McXmlWidgetBeanBuilderReader.h \
    $$PWD/ApplicationContext/Impl/McXmlWidgetApplicationContext.h \
    $$PWD/ApplicationContext/Impl/McLocalPathWidgetApplicationContext.h

SOURCES += \
    $$PWD/McWidgetGlobal.cpp \
    $$PWD/BeanBuilder/Impl/McWidgetBeanBuilder.cpp \
    $$PWD/BeanBuilderReader/Impl/McXmlWidgetBeanBuilderReader.cpp \
    $$PWD/ApplicationContext/Impl/McXmlWidgetApplicationContext.cpp \
    $$PWD/ApplicationContext/Impl/McLocalPathWidgetApplicationContext.cpp
