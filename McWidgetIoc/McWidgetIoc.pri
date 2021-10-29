QT *= widgets

include($$PWD/../3rdparty/framelesshelper.pri)
# include($$PWD/../3rdparty/AdvancedDockingSystem/AdvancedDockingSystem.pri)

INCLUDEPATH += $$PWD/include/

SOURCES += \
    $$PWD/src/ApplicationContext/McAbstractWidgetApplicationContext.cpp \
    $$PWD/src/ApplicationContext/McXmlWidgetApplicationContext.cpp \
    $$PWD/src/BeanFactory/McAbstractWidgetBeanFactory.cpp \
    $$PWD/src/BeanFactory/McDefaultWidgetBeanFactory.cpp \
    $$PWD/src/McBuiltInBeanBuilder/McBuiltInBeanContainer.cpp \
    $$PWD/src/McWidgetGlobal.cpp \
    $$PWD/src/Window/McMainWindow.cpp \
    $$PWD/src/Window/McWidget.cpp

HEADERS += \
    $$PWD/include/McWidgetIoc/ApplicationContext/IMcWidgetApplicationContext.h \
    $$PWD/include/McWidgetIoc/ApplicationContext/Impl/McAbstractWidgetApplicationContext.h \
    $$PWD/include/McWidgetIoc/ApplicationContext/Impl/McXmlWidgetApplicationContext.h \
    $$PWD/include/McWidgetIoc/BeanFactory/IMcConfigurableWidgetBeanFactory.h \
    $$PWD/include/McWidgetIoc/BeanFactory/IMcWidgetBeanFactory.h \
    $$PWD/include/McWidgetIoc/BeanFactory/Impl/McAbstractWidgetBeanFactory.h \
    $$PWD/include/McWidgetIoc/BeanFactory/Impl/McDefaultWidgetBeanFactory.h \
    $$PWD/include/McWidgetIoc/BuiltInBeanBuilder/IMcBuiltInBeanBuilder.h \
    $$PWD/include/McWidgetIoc/BuiltInBeanBuilder/IMcBuiltInBeanBuilderFactory.h \
    $$PWD/include/McWidgetIoc/BuiltInBeanBuilder/Impl/McBuiltInBeanBuilder.h \
    $$PWD/include/McWidgetIoc/BuiltInBeanBuilder/Impl/McBuiltInBeanBuilderFactory.h \
    $$PWD/include/McWidgetIoc/BuiltInBeanBuilder/McBuiltInBeanContainer.h \
    $$PWD/include/McWidgetIoc/McWidgetGlobal.h \
    $$PWD/include/McWidgetIoc/McWidgetMacroGlobal.h \
    $$PWD/include/McWidgetIoc/Window/McMainWindow.h \
    $$PWD/include/McWidgetIoc/Window/McWidget.h
