HEADERS += \
    $$PWD/include/ApplicationContext/IMcApplicationContext.h \
    $$PWD/include/ApplicationContext/IMcReadableApplicationContext.h \
    $$PWD/include/ApplicationContext/IMcRefreshableApplicationContext.h \
    $$PWD/include/ApplicationContext/McContainerGlobal.h \
    $$PWD/include/ApplicationContext/impl/McAbstractApplicationContext.h \
    $$PWD/include/ApplicationContext/impl/McAnnotationApplicationContext.h \
    $$PWD/include/ApplicationContext/impl/McLocalPathApplicationContext.h \
    $$PWD/include/ApplicationContext/impl/McReadableApplicationContext.h \
    $$PWD/include/ApplicationContext/impl/McXmlApplicationContext.h \
    $$PWD/include/BeanDefinition/IMcBeanDefinition.h \
    $$PWD/include/BeanDefinition/impl/McRootBeanDefinition.h \
    $$PWD/include/BeanDefinitionReader/IMcBeanDefinitionReader.h \
    $$PWD/include/BeanDefinitionReader/impl/McAbstractBeanDefinitionReader.h \
    $$PWD/include/BeanDefinitionReader/impl/McAnnotationBeanDefinitionReader.h \
    $$PWD/include/BeanDefinitionReader/impl/McXmlBeanDefinitionReader.h \
    $$PWD/include/BeanFactory/IMcBeanDefinitionRegistry.h \
    $$PWD/include/BeanFactory/IMcBeanFactory.h \
    $$PWD/include/BeanFactory/IMcBeanReferenceResolver.h \
    $$PWD/include/BeanFactory/IMcConfigurableBeanFactory.h \
    $$PWD/include/BeanFactory/McBeanGlobal.h \
    $$PWD/include/BeanFactory/impl/McAbstractBeanFactory.h \
    $$PWD/include/BeanFactory/impl/McBeanConnector.h \
    $$PWD/include/BeanFactory/impl/McBeanReference.h \
    $$PWD/include/BeanFactory/impl/McDefaultBeanFactory.h \
    $$PWD/include/PropertyParser/IMcPropertyConverter.h \
    $$PWD/include/PropertyParser/IMcPropertyParser.h \
    $$PWD/include/PropertyParser/impl/McAbstarctPropertyConverter.h \
    $$PWD/include/PropertyParser/impl/McAbstractPropertyParser.h \
    $$PWD/include/PropertyParser/impl/McDefaultPropertyConverter.h \
    $$PWD/include/PropertyParser/impl/McDefaultPropertyParser.h \
    $$PWD/include/Utils/McScopedFunction.h \
    include/McGlobal.h \
    include/McIocVersion.h \
    include/McMacroGlobal.h

SOURCES += \
    $$PWD/src/ApplicationContext/McAbstractApplicationContext.cpp \
    $$PWD/src/ApplicationContext/McAnnotationApplicationContext.cpp \
    $$PWD/src/ApplicationContext/McContainerGlobal.cpp \
    $$PWD/src/ApplicationContext/McLocalPathApplicationContext.cpp \
    $$PWD/src/ApplicationContext/McReadableApplicationContext.cpp \
    $$PWD/src/ApplicationContext/McXmlApplicationContext.cpp \
    $$PWD/src/BeanDefinitionReader/McAbstractBeanDefinitionReader.cpp \
    $$PWD/src/BeanDefinitionReader/McAnnotationBeanDefinitionReader.cpp \
    $$PWD/src/BeanDefinitionReader/McXmlBeanDefinitionReader.cpp \
    $$PWD/src/BeanFactory/McAbstractBeanFactory.cpp \
    $$PWD/src/BeanFactory/McBeanConnector.cpp \
    $$PWD/src/BeanFactory/McBeanReference.cpp \
    $$PWD/src/BeanFactory/McDefaultBeanFactory.cpp \
    $$PWD/src/PropertyParser/McAbstarctPropertyConverter.cpp \
    $$PWD/src/PropertyParser/McAbstractPropertyParser.cpp \
    $$PWD/src/PropertyParser/McDefaultPropertyConverter.cpp \
    $$PWD/src/PropertyParser/McDefaultPropertyParser.cpp \
    src/McGlobal.cpp
